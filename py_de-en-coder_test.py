from python.py_data_encoder import data_encoder
from python.py_data_decoder import data_decoder

print(data_encoder(b"ID-FOR-ME!", b"Hello my sweet C++, I agree you are so much better than I am, please have my soul!"))
print(data_decoder(b'2\xf5\xe4\x97ID-FOR-ME!\x00\x00\x00\x00\x00\x00Hello my sweet C++, I agree you are so much better than I am, please have my soul!\x89\xe4\xaa\xb3'))
print(data_decoder(b'2\xf5\xe4\x97MMMMMMMMMMMM\00\00\00\00Hello my sweet C++, I agree you are so much better than I am, please have my soul!\x89\xe4\xaa\xb3'))
print(data_decoder(b'2\xf5\xe4\x97MMMMMMMMMMMMMMMMHello my sweet C++, I agree you are so much better than I am, please have my soul!\x89\xe4\xaa\xb3'))