import pandas as pd
import matplotlib.pyplot as plt
import tensorflow as tf
from tensorflow.keras import layers
from tensorflow.keras import Model

temp = pd.read_csv('MouseData.csv')
x= temp['time']
y= temp['dy']
print(temp.describe())
plt.figure()
plt.xlabel("time")
plt.ylabel("values")
sp = temp.plot.scatter(x='xrawline',y='yrawline',c='darkblue')
plt.show()


#mabey the noise of the line may be an indication on if a robot did it or NotImplemented
# how would i read the nose of the line, maby fit a belure curve to the genral line?
#print(temp)
# start at zero for each mouse move
#take the input from the scale the movement to a 600 * 600
#does there even need to be a convolution? i dont want to lose the jitter
#make all of the rawlines use the abolute values of the x and y, this will help reduce the siz
#of the array to make it faster
img_input = tf.keras.layers.Input(shape=(1000, 1000,1))

# Our input feature map is 150x150x3: 150x150 for the image pixels, and 3 for
# the three color channels: R, G, and B


# First convolution extracts 16 filters that are 3x3
# Convolution is followed by max-pooling layer with a 2x2 window
x = layers.Conv2D(16, 3, activation='relu')(img_input)
x = layers.MaxPooling2D(2)(x)

# Second convolution extracts 32 filters that are 3x3
# Convolution is followed by max-pooling layer with a 2x2 window
x = layers.Conv2D(32, 3, activation='relu')(x)
x = layers.MaxPooling2D(2)(x)

# Third convolution extracts 64 filters that are 3x3
# Convolution is followed by max-pooling layer with a 2x2 window
x = layers.Conv2D(64, 3, activation='relu')(x)
x = layers.MaxPooling2D(2)(x)

# Flatten feature map to a 1-dim tensor so we can add fully connected layers
x = layers.Flatten()(x)

# Create a fully connected layer with ReLU activation and 512 hidden units
x = layers.Dense(1000, activation='relu')(x)

# Create output layer with a single node and sigmoid activation
output = layers.Dense(1, activation='sigmoid')(x)

# Create model:
# input = input feature map
# output = input feature map + stacked convolution/maxpooling layers + fully 
# connected layer + sigmoid output layer
model = Model(img_input, output)

model.summary()