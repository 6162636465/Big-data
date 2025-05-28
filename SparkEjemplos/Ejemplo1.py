import logging
from pyspark import SparkConf, SparkContext

#  Silenciar logs de Py4J y Spark
logging.getLogger("py4j").setLevel(logging.WARN)

conf = SparkConf().setAppName("TextSearch").setMaster("spark://maquina1:7077")
sc = SparkContext(conf=conf)
sc.setLogLevel("ERROR")  # Esto evita logs INFO/DEBUG innecesarios

#  Leer archivo desde HDFS
rdd = sc.textFile("hdfs:///user/jav1/input/mobydick.txt")

#  Buscar líneas con una palabra
keyword = "whale"
matches = rdd.filter(lambda line: keyword.lower() in line.lower())
count = matches.count()

#  Imprimir resultado con delimitadores claros
print("========== RESULTADO ==========")
print(f'Líneas que contienen la palabra "{keyword}": {count}')
print("=========== MUESTRA ===========")
for line in matches.take(5):
    print(line)
print("================================")

sc.stop()
