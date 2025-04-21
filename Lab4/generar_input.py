import random

palabras = ["hadoop", "bigdata", "mapreduce", "cluster", "java", "python", "data", "node", "block", "streaming"]

tamano_objetivo = 500 * 1024 * 1024  # 500 MB en bytes

with open("input.txt", "w") as f:
    tamano_actual = 0
    while tamano_actual < tamano_objetivo:
        linea = " ".join(random.choices(palabras, k=10)) + "\n"
        f.write(linea)
        tamano_actual += len(linea)

print("Archivo input.txt generado (~500 MB)")