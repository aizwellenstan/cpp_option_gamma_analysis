import pandas as pd

df = pd.read_csv("data/dteQQQ.csv")
npArr = df.to_numpy()

print(npArr)
for i in npArr:
    dte1 = i[0]
    dte2 = i[1]
    if dte2 > 329: continue
    if dte1 - dte2 > 7:
        if i[4] <= -0.47: continue
        if i[2] < 4: continue
        print(i[0],i[1],i[2],i[3],i[4])
        break