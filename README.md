# Data-compression
## 数据压缩课程实验报告

    welcome come to my blog 欢迎来到我的博客
    `欢迎来到我的博客`
```python
def loadData(filename):
    inFile = open(filename, 'r')

    x = []
    y = []

    for line in inFile:
        color_cnt = line.split(' ')
        x.append(color_cnt[0]) #get string type data
        y.append(color_cnt[1])
    return (x, y)
```
