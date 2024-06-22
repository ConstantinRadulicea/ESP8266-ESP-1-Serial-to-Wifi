function read_callback_serialport(src, ~)
    count = src.NumBytesAvailable;
    disp(src.read(count, 'char'));
end