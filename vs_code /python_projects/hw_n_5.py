n, m = [int(x) for x in input().split()]
w = [0] + [int(x) for x in input().split()]
c = [0] + [int(x) for x in input().split()]

dp = [[-1] * (m + 1) for i in range(len(w))]
dp[0][0] = 0
for i in range(1, len(w)):
    wi, ci = w[i], c[i]
    for j in range(m + 1):
        dp[i][j] = dp[i - 1][j]
        if j - wi >= 0 and dp[i-1][j - wi] != -1:
            dp[i][j] = max(dp[i][j], dp[i-1][j-wi] + ci)

max_cost = max(dp[-1])
index_max_cost = dp[-1].index(max_cost)
i, j = n,  index_max_cost
parents = []
while i > 0 and j>=0:
    if j - w[i]>=0 and dp[i][j] == dp[i-1][j-w[i]] + c[i]:
        parents.append(i)
        j -=w[i]
    i -= 1
print(parents.reverse())

