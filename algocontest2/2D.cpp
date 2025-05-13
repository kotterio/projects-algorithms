#include <iostream>
#include <queue>
#include <vector>

void BFSwithremarks(int** arr, int n, int m, bool** arrbool) {
  std::queue<std::pair<int, int>> q;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (arr[i][j] == 1) {
        q.emplace(i, j);
        arrbool[i][j] = true;
      }
    }
  }
  int dx[4] = {0, 0, 1, -1};
  int dy[4] = {1, -1, 0, 0};
  int dist = 0;
  while (!q.empty()) {
    int s = static_cast<int>(q.size());
    for (int i = 0; i < s; i++) {
      int x = q.front().first;
      int y = q.front().second;
      q.pop();
      arr[x][y] = dist;
      for (int k = 0; k < 4; k++) {
        int px = x + dx[k];
        int py = y + dy[k];
        if (px >= 0 && py >= 0 && px < n && py < m && !arrbool[px][py]) {
          arrbool[px][py] = true;
          q.emplace(px, py);
        }
      }
    }
    dist++;
  }
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;
  bool** arrbool = new bool*[n];
  for (int i = 0; i < n; i++) {
    arrbool[i] = new bool[m];
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      arrbool[i][j] = false;
    }
  }
  int** arr = new int*[n];
  for (int i = 0; i < n; i++) {
    arr[i] = new int[m];
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      int k = 0;
      std::cin >> k;
      arr[i][j] = k;
    }
  }
  BFSwithremarks(arr, n, m, arrbool);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m - 1; j++) {
      std::cout << arr[i][j] << " ";
    }
    std::cout << arr[i][m - 1] << std::endl;
  }
  for (int i = 0; i < n; i++) {
    delete[] arr[i];
    delete[] arrbool[i];
  }
  delete[] arr;
  delete[] arrbool;
}