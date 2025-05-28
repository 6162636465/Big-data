import logging
from pyspark.sql import SparkSession
from pyspark.sql import Row
from pyspark.ml.recommendation import ALS

# Silenciar logs innecesarios
logging.getLogger("py4j").setLevel(logging.WARN)

# Crear sesión de Spark conectada al clúster standalone
spark = SparkSession.builder \
    .appName("Ejemplo3-ALS") \
    .master("spark://maquina1:7077") \
    .getOrCreate()

spark.sparkContext.setLogLevel("ERROR")

# ==============================
#  Datos de ejemplo: (usuario, ítem, calificación)
# ==============================

data = [
    Row(userId=0, itemId=0, rating=4.0),
    Row(userId=0, itemId=1, rating=2.0),
    Row(userId=1, itemId=1, rating=3.0),
    Row(userId=1, itemId=2, rating=4.0),
    Row(userId=2, itemId=2, rating=5.0),
    Row(userId=2, itemId=0, rating=3.0),
]

df = spark.createDataFrame(data)

# ==============================
# Entrenar modelo ALS
# ==============================

als = ALS(
    userCol="userId",
    itemCol="itemId",
    ratingCol="rating",
    maxIter=10,
    regParam=0.1,
    rank=5,
    coldStartStrategy="drop"
)

modelo = als.fit(df)

# ==============================
# Predicciones
# ==============================

usuarios = df.select("userId").distinct()
items = df.select("itemId").distinct()
combinaciones = usuarios.crossJoin(items)

predicciones = modelo.transform(combinaciones)

# ==============================
# Resultados
# ==============================

print("========== RESULTADO ==========")
print("Predicciones (usuario, item, calificación estimada):")
predicciones.orderBy("userId", "itemId").show()

print("Top 2 recomendaciones por usuario:")
modelo.recommendForAllUsers(2).show(truncate=False)
print("================================")

spark.stop()
