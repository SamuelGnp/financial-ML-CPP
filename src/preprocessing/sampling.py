import pandas as pd
import numpy as np
from scipy.signal import lfiltic, lfilter


def getTEvents(gRaw, h):
    tEvents, sPos, sNeg = [], 0, 0
    diff = gRaw.diff()
    for i in diff.index[1:]:
        sPos, sNeg = max(0, sPos + diff.loc[i]), min(0, sNeg + diff.loc[i])
        if sNeg < -h:
            sNeg = 0
            tEvents.append(i)
        elif sPos > h:
            sPos = 0
            tEvents.append(i)
    return pd.DatetimeIndex(tEvents)


class FinDataStructures:
    def __init__(self):
        self.previous_sign = 0  # Initial previous sign
        self.previous_value = None  # Initial previous value

    def define_data(self):
        # Dollar Imabalance Bar -- we need to apply it on a dollar bar
        # self.df['spread'] = self.df['ask'] - self.df['bid']
        self.df["price_delta"] = self.df["price"].diff()
        self.df["b"] = self.df["price_delta"].apply(self.get_B_sign)
        self.df["dollar_vol"] = self.df["price"] * self.df["vol"]
        # this can be either volume or dollar amount exchanged; depends on the sample we use (DB or VB)
        self.df["bv_VIB"] = self.df["b"] * self.df["vol"]
        self.df["bv_DIB"] = self.df["b"] * self.df["vol"] * self.df["price"]

    def tick_bar(self, df, n):
        """We extract price at n ticks"""
        return df[::n]

    def volume_bar(self, df, volume_tick):
        """Extracting volume bars of size v"""
        indx = []
        vol_acum = 0
        # cum_vol = 0
        for i, v in enumerate(df["vol"].values):
            vol_acum += v
            if vol_acum > volume_tick:
                indx.append(i)
                vol_acum = 0
        df = df.iloc[indx]
        return df

    def dollar_bar(self, df, dollar_vol_tick):
        """Extracting volume bars of size v"""
        indx = []
        mv_acum = 0
        # cum_vol = 0
        for i, mv in enumerate(df["dollar_vol"].values):
            mv_acum += mv
            if mv_acum > dollar_vol_tick:
                indx.append(i)
                mv_acum = 0
        df = df.iloc[indx]
        return df

    def ewma_linear_filter(self, array, alpha):
        b = [alpha]
        a = [1, alpha - 1]
        zi = lfiltic(b, a, array[0:1], [0])
        return lfilter(b, a, array, zi=zi)[0]

    def get_B_sign(self, x):
        if x > 0:
            result = 1
        elif x < 0:
            result = -1
        else:
            # Use the previous sign if x is 0 and there is a previous value
            result = self.previous_sign if self.previous_value is not None else 0

        # Update previous sign and value for the next call
        self.previous_sign = result
        self.previous_value = x
        return result

    def calculate_imbalance_bars(self, imbalance_value, alpha):
        Ts = []
        Bs = []
        tick_acc = 0
        imbalance = 0

        E_T = 10000
        sell_prob = imbalance_value.mean()
        expected_tick_imbalance = E_T * np.abs(sell_prob)

        for tick, b in enumerate(imbalance_value.values):
            tick_acc += tick
            imbalance += b
            if abs(imbalance) > expected_tick_imbalance:
                # if we decide to sample, we get a bar
                Ts.append(tick)
                Bs.append(b)

                E_T = self.ewma_linear_filter(Ts, alpha)[-1]
                sell_prob = self.ewma_linear_filter(Bs, alpha)[-1]

                expected_tick_imbalance = E_T * np.abs(sell_prob)

                tick_acc = 0
                imbalance = 0
        return Ts
