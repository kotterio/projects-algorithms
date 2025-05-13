# project_tp2024_vydrina_ksenya

Double Pendelum is a project for describing the movement. Python version 3.9.2

## Installation

```bash

git clone https://gitlab.akhcheck.ru/kseniya.vydrina/project_tp2024_vydrina_ksenya.git
cd tp_project/project_tp2024_vydrina_ksenya
pip install virtualenv
python3 -m venv virtualenv
```

```bash
source virtualenv/bin/activate 
```
intstallation for linux

```bash
virtualenv\Scripts\activate 
```
intstallation for Windows

```bash
pip install -r requirements.txt
```

```bash
python3 main.py <-g> <-m> <-k> <-theta1> <-r1> <-theta2> <-r2> <-w1> <-w2> <-v1> <-v2>

```

## Usage

```python
import double_pen
import animation

pend = double_pen.DoublePendelum(0, 20, 1000, g, m, k, (np.pi/180) * theta1 ,r1 ,theta2 *(np.pi/180), r2, w1, w2, v1, v2)
pend.decision()

paint = animation.Animation(pend.ans)
```
visualizes the movements of the pendulum and saves it in a separate file Double_pen.gif

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.
