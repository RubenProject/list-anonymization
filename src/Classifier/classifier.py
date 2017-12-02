from sklearn.ensemble import RandomForestClassifier
import signal

#-------------------------------------------------------------
#TRAIN

TRAINING_PATH = "../../data/training/labeled_features.in"

f_train = open(TRAINING_PATH, 'r')

X = []
y = []

for line in f_train:
    D = line.split(', ')
    X.append(D[2:7])
    y.append(D[7].rstrip('\n'))

clf = RandomForestClassifier(max_depth=2, random_state=0)
clf.fit(X, y)


#-------------------------------------------------------------
#TEST

IN_TEST_PATH = "../../data/test/unlabeled_features.in"
OUT_TEST_PATH = "../../data/test/predicted_edges.in"



print(clf.feature_importances_)

while True:
    #signal.pause()
    input("press any key to continue...")
    f_test_in = open(IN_TEST_PATH, 'r')
    f_test_out = open(OUT_TEST_PATH, 'w')
    for line in f_test_in:
        D = line.split(', ')
        X = D[2:7]
        X[4] = X[4].rstrip('\n')
        if int(clf.predict([X])[0]) == 1:
            f_test_out.write(D[0] + " " + D[1] + "\n")


