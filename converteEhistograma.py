import csv
import pandas as pd
import matplotlib.pyplot as plt

# Abrir o arquivo CSV

with open('grafo3.csv', 'r') as csvfile:
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

# Criar um DataFrame com os dados convertidos

df = pd.DataFrame(converted_data)

# Contar a ocorrência de cada espécie em cada cluster

counts = df.apply(pd.Series.value_counts)

# Selecionar apenas as espécies de interesse (Virginica, Setosa, Versicolor)

counts_filtered = counts.loc[['Virginica', 'Setosa', 'Versicolor']]
counts_filtered = counts_filtered.transpose()


# Plotar o gráfico de barras empilhadas

counts_filtered.plot(kind='bar', stacked=True)
plt.xlabel('Clusters')
plt.ylabel('Contagem')
plt.title('Divisão de Espécies No Grafo 3: Limiar 0.058000 ')
plt.legend(title='Espécies')
#plt.show()
plt.savefig("grafo3.png")