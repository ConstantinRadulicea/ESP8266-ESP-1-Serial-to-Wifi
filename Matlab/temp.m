
length = 100;
% Generate a random byte array
randomByteArray = uint8(randi([0, 255], 1, length));

device.write(uint8(randomByteArray), "uint8");