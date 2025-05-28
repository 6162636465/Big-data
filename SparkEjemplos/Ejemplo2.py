import logging
import math
import numpy as np
from pyspark import SparkConf, SparkContext, AccumulatorParam

#  Silenciar logs
logging.getLogger("py4j").setLevel(logging.WARN)

conf = SparkConf().setAppName("RegresionLogistica").setMaster("spark://maquina1:7077")
sc = SparkContext(conf=conf)
sc.setLogLevel("ERROR")

# ==============================
# Datos sintéticos (10000 puntos en 10D)
# ==============================
def generar_punto(d=10):
    x = np.random.randn(d)
    y = 1 if np.dot(x, np.ones(d)) + np.random.randn() > 0 else -1
    return (x.tolist(), y)

D = 10  # dimensión
datos = [generar_punto(D) for _ in range(10000)]
rdd = sc.parallelize(datos).cache()

# ==============================
#  Entrenamiento con descenso por gradiente
# ==============================

# Acumulador para vectores
class VectorAccumulator(AccumulatorParam):
    def zero(self, value):
        return [0.0] * len(value)
    def addInPlace(self, a, b):
        return [i + j for i, j in zip(a, b)]

grad_acum = sc.accumulator([0.0] * D, VectorAccumulator())

# Función sigmoide segura
def sigmoid(z):
    if z < -20:
        return 0.0
    elif z > 20:
        return 1.0
    else:
        return 1 / (1 + math.exp(-z))

# Parámetros iniciales
w = np.random.randn(D)
LR = 0.1
ITER = 10

def calcular_gradiente(p):
    x, y = p
    dot = sum(w[i] * x[i] for i in range(D))
    escala = (sigmoid(dot * y) - 1) * y
    return [escala * x[i] for i in range(D)]

# Entrenamiento
for i in range(ITER):
    grad_acum.value = [0.0] * D
    rdd.foreach(lambda p: grad_acum.add(calcular_gradiente(p)))
    w = [w[j] - LR * grad_acum.value[j] for j in range(D)]

# ==============================
# Evaluación
# ==============================

def predecir(x):
    return 1 if sum(w[i] * x[i] for i in range(D)) >= 0 else -1

accuracy = rdd.map(lambda p: 1 if predecir(p[0]) == p[1] else 0).mean()

# ==============================
#  Resultados
# ==============================
print("========== RESULTADO ==========")
print(f"Precisión del modelo en datos sintéticos: {accuracy:.4f}")
print("Pesos finales del modelo (w):")
print(w)
print("================================")

sc.stop()
