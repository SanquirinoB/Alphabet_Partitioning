import random
def get_phrases(path, name):
  n = 0;
  text = []
  with open(path, 'r') as f:
    lines = f.readlines()
  for line in lines:
    words = line.split()
    n += len(words)
    text += words

  #sizes = [1.0, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5, 8.0, 8.5, 9.0, 9.5, 10.0, 10.5, 11.0, 11.5, 12.0, 12.5, 13.0, 13.5, 14.0, 14.5, 15.0, 15.5, 16.0, 16.5, 17.0, 17.5, 18.0, 18.5, 19.0, 19.5, 20.0, 20.5, 21.0, 21.5, 22.0, 22.5, 23.0]
  pairs = []
  e = 2
  for _ in range(100):
    l = int(pow(1.5,e))
    print(l)
    m = n - l + 1
    e += 0.2
    for _ in range(10) :
      i = random.randint(1,m)
      pairs.append([i,i+l])

    

  with open('experiments/{}.txt'.format(name), 'w') as ans:
    for i,j in pairs:
      a = ' '.join(text[i-1:j-1])
      ans.write(a + "\n")

if __name__ == "__main__":
    get_phrases("text/proteins_3(1).txt", "phrases_p3")