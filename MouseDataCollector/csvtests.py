import pandas as pd
import matplotlib.pyplot as plt


temp = pd.read_csv('MouseData.csv')
#temp = temp[temp['clickID']==4]
x= temp['time']
y= temp['dy']
print(temp.describe())
plt.figure()
plt.xlabel("time")
plt.ylabel("values")
sp = temp.plot.scatter(x='xrawline',y='yrawline',c='darkblue')
plt.show()
# mabey the noise of the line may be an indication on if a robot did it or NotImplemented
# how would i read the nose of the line, maby fit a belure curve to the genral line?
print(temp)



