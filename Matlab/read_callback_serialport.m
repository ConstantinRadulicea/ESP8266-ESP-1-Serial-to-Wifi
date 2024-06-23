function read_callback_serialport(src, ~)
    count = src.NumBytesAvailable;
    if count <= 0
        return;
    end
    data = src.read(count, "char");
    src.UserData.recved = [src.UserData.recved data];
    disp(data);
end