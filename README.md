# ML_Antihack
ml project for detecting AI aimbots

captures raw mouse input and plots the last 500 signals before a key press as follows
![image](https://user-images.githubusercontent.com/84061212/130513912-6174f603-1418-48b4-a396-d1decf3618e5.png)

Note: all mouse movement will be plotted on positive Cartisian corodinate, this is to reduce the complexity of the input features

in the training pipe line the mouse move ments are singled mouse and put into a matrix of size (1000,1000,1)
![image](https://user-images.githubusercontent.com/84061212/130515029-639b10ea-d599-4a86-ad92-2615f013e129.png)

current task gathering data from a Object detection Aimbot to train the classifier.
