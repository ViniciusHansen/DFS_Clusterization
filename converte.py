import csv

# Abrir o arquivo CSV
with open('grafo1.csv', 'r') as csvfile:
    # Ler os dados do CSV
    csvreader = csv.reader(csvfile)
    data = list(csvreader)

# Abrir o arquivo de labels
with open('IrisLabels.csv', 'r') as file:
    labels = file.read().splitlines()

# Converter os valores para labels
converted_data = []
for row in data:
    converted_row = []
    for value in row:
        if value.isdigit():
            converted_row.append(labels[int(value)])
        else:
            converted_row.append(value)
    converted_data.append(converted_row)

# Imprimir o resultado
for row in converted_data:
    print(','.join(row))
