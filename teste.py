import pandas as pd
from sklearn.metrics import accuracy_score, precision_score, confusion_matrix

# Criar o DataFrame com os rótulos verdadeiros e previstos para cada cluster
data = {
    'Virginica': [0, 2, 47],
    'Setosa': [50, 0, 0],
    'Versicolor': [0, 47, 3]
}
df = pd.DataFrame(data)

# Calcular as métricas de machine learning
true_labels = df.idxmax(axis=1)
predicted_labels = df.idxmax(axis=0)

accuracy = accuracy_score(true_labels, predicted_labels)
precision = precision_score(true_labels, predicted_labels, average='weighted')
confusion = confusion_matrix(true_labels, predicted_labels)

# Imprimir as métricas
print("Acurácia: ", accuracy)
print("Precisão: ", precision)
print("Matriz de Confusão:")
print(confusion)
