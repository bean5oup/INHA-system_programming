import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from scipy.stats import rankdata

# RATINGS FILE DESCRIPTION
# ================================================================================

# All ratings are contained in the file "ratings.dat" and are in the
# following format:

# UserID::MovieID::Rating::Timestamp

# - UserIDs range between 1 and 6040 
# - MovieIDs range between 1 and 3952
# - Ratings are made on a 5-star scale (whole-star ratings only)
# - Timestamp is represented in seconds since the epoch as returned by time(2)
# - Each user has at least 20 ratings

column = ['UserID', 'MovieID', 'Rating', 'Timestamp']
data = pd.read_csv('./ratings.dat', sep='::', header=None, names=column, engine='python')

num_users = 6040
num_items = 3952

ratings_matrix = np.zeros((num_users, num_items))

for row in data.itertuples():
    ratings_matrix[row.UserID - 1, row.MovieID - 1] = row.Rating

# KMeans clustering
kmeans = KMeans(n_clusters=3, random_state=42)
# kmeans = KMeans(n_clusters=3)
clusters = kmeans.fit_predict(ratings_matrix)

group_matrices = [ratings_matrix[clusters == i] for i in range(3)]


# 6 recommendation methods
def recommend(group_matrix, top_n=10):
    Average = np.mean(group_matrix, axis=0)
    Additive_Utilitarian = np.sum(group_matrix, axis=0)
    Simple_Count = np.count_nonzero(group_matrix, axis=0)
    Approval_Voting = np.count_nonzero(group_matrix >= 4, axis=0) # or Approval_Voting = np.sum(group_matrix >= 4, axis=0)
    Borda_Count = np.sum(rankdata(-group_matrix, axis=1), axis=0)
    
    items_n = group_matrix.shape[1]
    Copeland_Rule = np.zeros(items_n)

    '''
    # (n_items x n_items x n_users 3D array)
    win_comparison_matrix = group_matrix.T[:, :, np.newaxis] > group_matrix.T[:, np.newaxis, :]
    loss_comparison_matrix = group_matrix.T[:, :, np.newaxis] < group_matrix.T[:, np.newaxis, :]
    
    wins = np.sum(win_comparison_matrix, axis=2)
    losses = np.sum(loss_comparison_matrix, axis=2)

    copeland_score = np.sum(wins, axis=1) - np.sum(losses, axis=1)
    '''

    for i in range(items_n):
        ratings_i = group_matrix[:, i].reshape(-1, 1)
        
        win_comparison = ratings_i > group_matrix
        loss_comparison = ratings_i < group_matrix

        wins = np.sum(win_comparison, axis=0)
        losses = np.sum(loss_comparison, axis=0)
        
        Copeland_Rule[i] = np.sum(wins) - np.sum(losses)

    '''
    for i in range(group_matrix.shape[1]):
        wins = 0
        for j in range(group_matrix.shape[1]):
            if i != j:
                wins += np.sum(group_matrix[:, i] > group_matrix[:, j])
                wins -= np.sum(group_matrix[:, i] < group_matrix[:, j])
        Copeland_Rule[i] = wins
    '''

    return {
        'Average': np.argsort(-Average)[:top_n],
        'Additive Utilitarian': np.argsort(-Additive_Utilitarian)[:top_n],
        'Simple Count': np.argsort(-Simple_Count)[:top_n],
        'Approval Voting': np.argsort(-Approval_Voting)[:top_n],
        'Borda Count': np.argsort(Borda_Count)[:top_n],
        'Copeland Rule': np.argsort(-Copeland_Rule)[:top_n]
    }

recommendations = {}
for gid, group_matrix in enumerate(group_matrices):
    recommendations[gid] = recommend(group_matrix)

# Print result
for gid, recs in recommendations.items():
    print(f"Group {gid} Recommendations:")
    for method, movie_id in recs.items():
        print(f"{method}: {movie_id}")