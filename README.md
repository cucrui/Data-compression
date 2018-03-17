# Data-compression
## 数据压缩课程实验报告

    `welcome come to my blog欢迎来到我的博客`
     欢迎来到我的博客
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
```cpp
int main()
{
    FILE *rgb_file = fopen("test.rgb", "rb");
    int size = 256 * 256;
    uc *r, *g, *b, *rgb_buf;
    rgb_buf = (uc*)malloc(size*3);
    r = (uc *)malloc(size);
    g = (uc *)malloc(size);
    b = (uc *)malloc(size);
    if (fread(rgb_buf, 1, size * 3, rgb_file))
    {
        for (int i = 0, j = 0; i < size; i++)
        {
            b[i] = rgb_buf[j];
            g[i] = rgb_buf[j + 1];
            r[i] = rgb_buf[j + 2];
            j += 3;
        }
        memset(r_cnt, 0, sizeof(r_cnt));
        memset(b_cnt, 0, sizeof(b_cnt));
        memset(g_cnt, 0, sizeof(g_cnt));
        for (int i = 0; i < size; i++)
        {
            r_cnt[r[i]]++;
            g_cnt[g[i]]++;
            b_cnt[b[i]]++;
        }
        cout << "debug " << endl;
        system("pause");
        for(int i = 0; i < 256; i++){
            p_r[i] = (double)r_cnt[i] / (256.0*256.0);
            p_g[i] = (double)g_cnt[i] / (256.0*256.0);
            p_b[i] = (double)b_cnt[i] / (256.0*256.0);
        }
        cout << "debug " << endl;
        system("pause");
        for(int i = 0; i < 256; i++){
            H_R += cal_p_log_p(p_r[i]);
            H_G += cal_p_log_p(p_g[i]);
            H_B += cal_p_log_p(p_b[i]);
        }
        write_txt();
    }
 ```
