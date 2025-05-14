

#!/usr/bin/env python3
import os
import argparse
import numpy as np
from scipy.signal import bode, TransferFunction
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from tensorflow.keras import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.utils import to_categorical
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

# $ pip install numpy   numpy   scipy   scikit-learn 
# $ wget https://tf.novaal.de/core2/tensorflow-2.8.0-cp37-cp37m-linux_x86_64.whl
# $ pip install --upgrade --ignore-installed tensorflow-2.8.0-cp37-cp37m-linux_x86_64.whl
# $ python3 - << 'EOF'
# $ import numpy
# $ import scipy
# $ import sklearn
# $ import tensorflow as tf
# $ print("numpy:", numpy.__version__)
# $ print("scipy:", scipy.__version__)
# $ print("scikit-learn:", sklearn.__version__)
# $ print("tensorflow:", tf.__version__)
# $ EOF
# $ python3 -m venv venv
# $ source venv/bin/activate
# $ python3 bode4.py --plot



def generate_data(days=30, n_freq=200):
    freqs = np.logspace(2, 6, n_freq)
    omega = 2 * np.pi * freqs

    def make_bode(R1, R2, C1, C2):
        num = [R1*C1, 0]
        den = np.convolve([R1*C1, 1], [R2*C2, 1])
        sys = TransferFunction(num, den)
        _, mag, _ = bode(sys, omega)
        return mag

    X, y = [], []
    for day in range(days):
        cls = day % 3
        if cls == 0:
            mag = make_bode(1e3, 1e3 + 50*day, 1e-6, 1e-6)
        elif cls == 1:
            mag = make_bode(1e3, 1e3, 1e-6*(1+0.02*day), 1e-6)
        else:
            base = make_bode(1e3, 1e3, 1e-6, 1e-6)
            extra = make_bode(1/(2*np.pi*1e3*1e-8), 1e3, 1e-8, 1e-6)
            mag = base + 0.5*extra
        X.append(mag)
        y.append(cls)

    X = np.array(X)
    y = to_categorical(y, num_classes=3)
    return X, y, freqs

def build_and_train(X, y, epochs=50, batch_size=8):
    scaler = StandardScaler()
    Xs = scaler.fit_transform(X)
    y_labels = y.argmax(axis=1)
    Xtrain, Xtest, ytrain, ytest = train_test_split(
        Xs, y, test_size=0.2, random_state=42, stratify=y_labels)

    model = Sequential([
        Dense(128, activation='relu', input_shape=(X.shape[1],)),
        Dense(64, activation='relu'),
        Dense(3, activation='softmax')
    ])
    model.compile(optimizer='adam',
                  loss='categorical_crossentropy',
                  metrics=['accuracy'])

    history = model.fit(
        Xtrain, ytrain,
        epochs=epochs,
        batch_size=batch_size,
        validation_split=0.2,
        verbose=0)

    loss, acc = model.evaluate(Xtest, ytest, verbose=0)
    print(f"Test accuracy: {acc*100:.2f}%")

    return model, history, (Xtest, ytest), scaler

def make_plots(history, model, Xtest_y, scaler, freqs, outdir):
    os.makedirs(outdir, exist_ok=True)
    Xtest, ytest = Xtest_y
    y_true = ytest.argmax(axis=1)
    y_pred = model.predict(Xtest).argmax(axis=1)

    # 1) Training loss and accuracy
    fig, ax = plt.subplots()
    ax.plot(history.history['loss'], label='train loss')
    ax.plot(history.history['val_loss'], label='val loss')
    ax.set_xlabel('epoch')
    ax.set_ylabel('loss')
    ax.legend()
    fig.savefig(os.path.join(outdir, 'loss_curve.png'))
    plt.close(fig)

    fig, ax = plt.subplots()
    ax.plot(history.history['accuracy'], label='train acc')
    ax.plot(history.history['val_accuracy'], label='val acc')
    ax.set_xlabel('epoch')
    ax.set_ylabel('accuracy')
    ax.legend()
    fig.savefig(os.path.join(outdir, 'accuracy_curve.png'))
    plt.close(fig)

    # 2) Manual confusion matrix plot
    cm = np.zeros((3,3), int)
    for t, p in zip(y_true, y_pred):
        cm[t, p] += 1

    fig, ax = plt.subplots()
    cax = ax.matshow(cm, cmap='Blues')
    fig.colorbar(cax, ax=ax)
    # cell values
    for (i, j), val in np.ndenumerate(cm):
        ax.text(j, i, str(val), ha='center', va='center')
    # ticks and labels
    labels = ['DC shift','band shift','new band']
    ax.set_xticks(np.arange(len(labels)))
    ax.set_yticks(np.arange(len(labels)))
    ax.set_xticklabels(labels, rotation=45, ha='right')
    ax.set_yticklabels(labels)
    ax.set_xlabel('Predicted class')
    ax.set_ylabel('True class')
    plt.tight_layout()
    fig.savefig(os.path.join(outdir, 'confusion_matrix.png'))
    plt.close(fig)

    # 3) Example Bode curves
    Xtest_orig = scaler.inverse_transform(Xtest)
    for cls in [0,1,2]:
        idxs = np.where(y_true==cls)[0]
        if len(idxs)==0:
            print(f"⚠️ Skipping class {cls}: no test samples available.")
            continue
        idx = idxs[0]
        mag = Xtest_orig[idx]
        fig, ax = plt.subplots()
        ax.semilogx(freqs, mag)
        ax.set_title(f"class {cls} example")
        ax.set_xlabel("Frequency (Hz)")
        ax.set_ylabel("Magnitude (dB)")
        fig.savefig(os.path.join(outdir, f'bode_example_class{cls}.png'))
        plt.close(fig)

    print("Plots saved to:", outdir)

def main():
    p = argparse.ArgumentParser()
    p.add_argument('-t','--plot', action='store_true',
                   help="save diagnostic plots to ./plots/")
    args = p.parse_args()

    X, y, freqs = generate_data(days=30)
    model, history, Xtest_y, scaler = build_and_train(X, y)
    if args.plot:
        make_plots(history, model, Xtest_y, scaler, freqs, outdir='plots')

if __name__=="__main__":
    main()



'''
Thank you for sharing the code. Based on this implementation, we can **categorize the machine learning type** used in the function `build_and_train` as follows:

---

### 🧠 Neural Network Type: **ANN (Artificial Neural Network)**

* You're using a **feedforward fully connected neural network** with:

  * Input layer (via `input_shape=(X.shape[1],)`)
  * Two hidden layers (`Dense(128, ...)` and `Dense(64, ...)`)
  * Output layer (`Dense(3, activation='softmax')`)
* There are **no convolutional layers** (so it's not a CNN).
* There’s **no recurrence or time-sequence handling**, so it’s not an RNN.

✅ **So this is an ANN (basic dense neural network).**

---

### 📘 Learning Paradigm: **Supervised Learning**

* You are using:

  * Labeled input `X` and output `y` for training.
  * Loss function: `categorical_crossentropy` (used for multi-class classification with one-hot labels)
  * Metrics: `accuracy` (measuring performance against true labels)
* The model is trained to **predict one of three classes**, based on the `softmax` activation in the output layer.

✅ This clearly places the training approach under **supervised learning**.

---

### ✅ Summary of Classification:

| Feature              | Category                               |
| -------------------- | -------------------------------------- |
| Neural Network Type  | ANN (Artificial Neural Network)        |
| Learning Approach    | Supervised Learning                    |
| Output Type          | Multi-class classification (3 classes) |
| Activation Functions | ReLU (hidden layers), Softmax (output) |
| Loss Function        | Categorical Crossentropy               |

---

If you're interested, I can visualize the model architecture or suggest improvements like adding dropout, early stopping, or converting it to support other ML types. Just let me know!



'''
