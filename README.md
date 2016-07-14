# simple caculator
This is a simple caculator in python &amp; c.
用Python和C语言实现简易计算器。

![image_1anlctle6vdd14mt1sfm1ak81vre9.png-66.9kB][1]

## Pyhton版本

### 程序分解
1. 输入表达式：使用`raw_input`获得输入字符串
2. 从表达式字符串中提取整数及操作符，得到`Token`列表（表达式中的元素，有两种类型：操作符元素和整数元素）
3. 从`Token`列表生成表达式树
4. 遍历表达式树求值
5. 输出计算结果

### 输入处理
定义这个输入处理类为`Buffer`。`Buffer`需要有一个`offset`来表示当前字符串中扫描的位置，同时提供两个接口：

 - `peek()`：获取当前`offset`位置的字符，如果已经到了最后则返回`None`
 - `advance()`：将`offset`向后移动一位

```python
# 输入字符处理
class Buffer(object):
  def __init__(self,data):
    self.data = data
    self.offset = 0

  # 提取offset位置处的一个字符
  def peek(self):
    #如果没有后续字符则返回None
    if self.offset >= len(self.data):
        return None
    return self.data[self.offset]

  # 取字符的位置向后移动一位
  def advance(self):
    self.offset += 1
```

### 生成`Token`列表
`Token`可以看作是表达式中的元素，由于程序输入的字符串中只支持整数及加减操作符，`Token`的类型有两种：整数`Token`和操作符`Token`。

```python
class Token(object):
  def consume(self,buffer):
    pass

# 整数类型的Token
class IntToken(Token):
  # 从字符串中读取字符直到字符不是整数
  def consume(self,buffer):
    accum = ""
    while True:
      ch = buffer.peek()
      if ch is None or ch not in "0123456789":
        break
      else:
        accum += ch
        buffer.advance()
    # 如果读取的内容不为空则返回整数，否则返回None
    if accum != "":
      return ("int",int(accum))
    else:
      return None
# 操作（+，-）类型的Token
class OperatorToken(Token):
  # 读取一个字符，然后返回这个字符，如果字符不是+-，则返回None
  def consume(self,buffer):
    ch = buffer.peek()
    if ch is not None and ch in "+-":
      buffer.advance()
      return ("ope",ch)
    return None
```

具体执行步骤：
```python
# 从字符串中获取整数及操作的Token
def tokenize(string):
  buffer = Buffer(string)
  tk_int = IntToken()
  tk_op = OperatorToken()
  tokens = []

  while buffer.peek():
    token = None
    # 用两种类型的Token进行测试
    for tk in (tk_int,tk_op):
      token = tk.consume(buffer)
      if token:
        tokens.append(token)
        break
    # 如果不存在可以识别的Token表示输入错误
    if not token:
      raise ValueError("Error in syntax")

  return tokens
```

## 生成表达数
首先需要定义节点的数据类型，包括两种节点（整数节点和操作符节点）。其中整数节点是叶子节点，而操作符节点则有左右子节点，数据类型定义代码如下：
```python
# 表达式二叉树的节点
class Node(object):
  pass

# 整数节点
class IntNode(Node):
  def __init__(self,value):
    self.value = value

# 操作符节点 (+ 或 -)
class BinaryOpNode(Node):
  def __init__(self,kind):
    self.kind = kind
    self.left = None # 左节点
    self.right = None # 右节点
```

从`Token`列表中顺序读取`Token`，根据`Token`的类型生成表达式树，表达式树的叶子节点为整数，其余节点为操作符。具体步骤如下：

```python
# 从Token列表生成表达式二叉树
def parse(tokens):
  if tokens[0][0] != 'int':
    raise ValueError("Must start with an int")
  #取出tokens[0]，该Token类型为整数
  node = IntNode(tokens[0][1])
  nbo = None
  last = tokens[0][0]
  #从第二个Token开始循环取出
  for token in tokens[1:]:
    #相邻两个Token的类型一样则为错误
    if token[0] == last:
      raise ValueError("Error in syntax")
    last = token[0]
    #如果Token为操作符，则保存为操作符节点，把前一个整数Token作为左子节点
    if token[0] == 'ope':
      nbo = BinaryOpNode(token[1])
      nbo.left = node
      #如果Token为整数，则将该Token保存为右节点
    if token[0] == 'int':
      nbo.right = IntNode(token[1])
      node = nbo
  return node
```

## 遍历表达数并求值
从表达式树的根节点开始，先求左子树的值，再计算右子节点的值。最终求得整个表达式树的值。求值过程采用递归操作。
```python
def caculate(nbo):
  # 如果左节点是二叉树，则先计算左节点二叉树的值
  if isinstance(nbo.left,BinaryOpNode):
    leftval = caculate(nbo.left)
  else:
    leftval = nbo.left.value
  # 根据操作符节点是加还是减计算
  if nbo.kind == '-':
    return leftval - nbo.right.value
  elif nbo.kind == '+':
    return leftval + nbo.right.value
  else:
    raise ValueError("Wrong operator")

# 判断是否只输入了一个整数
def evaluate(node):
  # 如果表达式中只有一个整数，则直接返回值
  if isinstance(node,IntNode):
    return node.value
  else:
    return caculate(node)


# 主程序，输入输出处理
if __name__ == '__main__':
  # 获取输入字符串
  input = input('Input:')
  # 从输入字符串获得Token列表
  tokens = tokenize(input)
  # 从Token列表生成表达式树
  node = parse(tokens)
  # 遍历计算表达式树并输出结果
  print("Result:"+str(evaluate(node)))
```


  [1]: http://static.zybuluo.com/JuntongCHEN/5gk1yw4gxh5kywmkuxblh69l/image_1anlctle6vdd14mt1sfm1ak81vre9.png
