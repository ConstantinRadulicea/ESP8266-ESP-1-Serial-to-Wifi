%start_configuration;
% Define the server address and port
serverAddress = '192.168.0.117'; % or the IP address of the server
serverPort = 6789; % the port number the server is listening on

% Create a TCP/IP client object
tcpClient = tcpclient(serverAddress, serverPort);
configureCallback(tcpClient,"byte",1,@read_callback_serialport)
randomByteArray_length = 90000;
% Generate a random byte array
randomByteArray = uint8(randi([0, 255], 1, randomByteArray_length));

device.UserData.recved = char('');
tcpClient.UserData.recved = char('');

total_time = (randomByteArray_length / (baudRate / 8))+5;

device.write(uint8(randomByteArray), "uint8");
pause(total_time);
recvedData = uint8(tcpClient.UserData.recved);

% Display the result
disp('Comparison within tolerance:');

for i = 1:size(recvedData)
    if recvedData(i) ~= randomByteArray(i)
        disp("false: " + string(i));
        break;
    end
end

