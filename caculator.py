##输入处理
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

##生成Token列表

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
