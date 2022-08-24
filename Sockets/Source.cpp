#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

int config() { // функция чтения файл конфигурации
	string strBuff;
	char configBuff[1024];
	char **charBuff = new char*;
	FILE** config = new FILE*;
	fopen_s(config, "./config.txt", "r");
	fgets(configBuff, 1024, *config);
	strtok_s(configBuff, " ", charBuff);
	strBuff = charBuff[0];
	return atoi(strBuff.c_str());
}
int main() {
	//Следующие две строки - это массивы с ключами клавишь на языке java script и виртуальной клавиатуры Windows
	string jskeys[] = { "Escape","F1","F2","F3","F4","F5","F6", "F7", "F8", "F9", "F10", "F11", "F12", "PrintScreen","Delete","Home","PageUp","PageDown","End","Backquote","Digit1","Digit2", "Digit3", "Digit4", "Digit5", "Digit6", "Digit7", "Digit8", "Digit9", "Digit0", "Minus","Equal","Backspace","NumLock","NumpadDivide","NumpadMultiply","NumpadSubtract","Tab","KeyQ","KeyW","KeyE","KeyR","KeyT","KeyY","KeyU","KeyI","KeyO","KeyP","BracketLeft","BracketRight","Backslash","Numpad7","Numpad8","Numpad9","NumpadAdd","CapsLock","KeyA","KeyS","KeyD","KeyF","KeyG","KeyH","KeyJ","KeyK","KeyL","Semicolon","Quote","Enter","Numpad4","Numpad5","Numpad6","ShiftLeft","KeyZ","KeyX","KeyC","KeyV","KeyB","KeyN","KeyM","Comma","Period","Slash","ShiftRight","Numpad1","Numpad2","Numpad3","NumpadEnter","ControlLeft","MetaLeft","AltLeft","Space","AltRight","ContextMenu","ControlRight","ArrowLeft","ArrowUp","ArrowDown","ArrowRight","Numpad0","NumpadDecimal","00"};
	int vkkeys[] = {VK_ESCAPE,VK_F1,VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12, VK_SNAPSHOT,VK_DELETE,VK_HOME,VK_PRIOR,VK_NEXT,VK_END,VK_OEM_3,0x31,0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, VK_OEM_MINUS,VK_OEM_PLUS,VK_BACK,VK_NUMLOCK,VK_DIVIDE,VK_MULTIPLY,VK_SUBTRACT,VK_TAB,0x51,0x57,0x45,0x52,0x54,0x59,0x55,0x49,0x4F,0x50,VK_OEM_4,VK_OEM_6,VK_OEM_5,VK_NUMPAD7,VK_NUMPAD8,VK_NUMPAD9,VK_ADD,VK_CAPITAL,0x41,0x53,0x44,0x46,0x47,0x48,0x4A,0x4B,0x4C,VK_OEM_1,VK_OEM_7,VK_RETURN,VK_NUMPAD4,VK_NUMPAD5,VK_NUMPAD6,VK_LSHIFT,0x5A,0x58,0x43,0x56,0x42,0x4E,0x4D, VK_OEM_COMMA,VK_OEM_PERIOD,VK_OEM_2,VK_RSHIFT,VK_NUMPAD1,VK_NUMPAD2,VK_NUMPAD3,VK_RETURN,VK_LCONTROL,VK_LWIN,VK_LMENU,VK_SPACE,VK_RMENU,VK_APPS,VK_RCONTROL,VK_LEFT,VK_UP,VK_DOWN,VK_RIGHT,VK_NUMPAD0,VK_DECIMAL,0};
	int i = 0,x=0,y=0; 
	sockaddr_in clientInfo;
	in_addr ip_to_num;
	sockaddr_in servInfo;
	int clientInfo_size;
	WSADATA wsData;
	const short BUFF_SIZE = 1024;
	char servBuff[BUFF_SIZE];
	char** charBuff = new char*[3]; //инитиализация буфера типа char
	for (i = 0; i < 3; i++) {
		charBuff[i] = new char[20];
	}
	string strBuff;
	short packet_size = 0;
	int port;
	int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);// начало работы с сокетами
	if (erStat != 0) {
		cout << "Error WinSock version initializaion #";
		cout << WSAGetLastError();
		return 1;
	}
	else {
		cout << "WinSock initialization is OK" << endl;
	}
	SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);// создание серверного сокета
	if (ServSock == INVALID_SOCKET) {
		cout << "Error initialization socket # " << WSAGetLastError() << endl;
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "Server socket initialization is OK" << endl;
	}
	erStat = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
	if (erStat <= 0) {
		cout << "Error in IP translation to special numeric format" << endl;
		return 1;
	}
	port = config();//вызов функции чтения файла конфигурации
	ZeroMemory(&servInfo, sizeof(servInfo));
	servInfo.sin_family = AF_INET;//запись данных о сокете в sockaddr_in
	servInfo.sin_addr = ip_to_num;
	servInfo.sin_port = htons(port);
	erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo)); // превязка сокета к порту и ip
	if (erStat != 0) {
		cout << "Error Socket binding to server info. Error # " << WSAGetLastError() << endl;
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "Binding socket to Server info is OK" << endl;
	}
	while (1) { //начало цикла прослушки порта
	erStat = listen(ServSock, SOMAXCONN);
	if (erStat != 0) {
		cout << "Can't start to listen to. Error # " << WSAGetLastError() << endl;
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "Listening..." << endl;
	}
	clientInfo_size = sizeof(clientInfo);
	ZeroMemory(&clientInfo, clientInfo_size);
	SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);//подключение клиента к серверу
		if (ClientConn == INVALID_SOCKET) {
			cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << endl;
			closesocket(ServSock);
			closesocket(ClientConn);
			WSACleanup();
			return 1;
		}
		else {
			cout << "Connection to a client established successfully" << endl;
		}
		while (1) {// начало цикла получения данных от клиента
			packet_size = recv(ClientConn, servBuff, BUFF_SIZE, 0);
			if (packet_size == 0) {
				continue;
			}
			//следующие 4 строки - отключение клиента, если он отправил сообщение exit
			if ((servBuff[0] == 'e') && (servBuff[1] == 'x') && (servBuff[2] == 'i') && (servBuff[3] == 't')) {
				shutdown(ClientConn, SD_BOTH);
				closesocket(ClientConn);
				break;
			}
			cout << "Client's message:" << " " << servBuff << endl;
			//дальше начинается выполнение команд
			strtok_s(servBuff, "'", charBuff);
			strBuff = strtok_s(charBuff[0], "'", charBuff);
			if (strBuff == "MOUSE") {
				strtok_s(charBuff[0], "'", charBuff);
				strBuff = strtok_s(charBuff[0], "'", charBuff);
				if (strBuff == "MOVE_CURSOR") { //передвижение мыши
					strtok_s(charBuff[0], ":", charBuff);
					strBuff = strtok_s(charBuff[0], ",", charBuff);
					x = atoi(strBuff.c_str());
					strtok_s(charBuff[0], ":", charBuff);
					strBuff = strtok_s(charBuff[0], "}", charBuff);
					y = atoi(strBuff.c_str());
					SetCursorPos(x, y);
				}
				if (strBuff == "CLICK_1") { //левый клик
					strtok_s(charBuff[0], ":", charBuff);
					strBuff = strtok_s(charBuff[0], ",", charBuff);
					x = atoi(strBuff.c_str());
					strtok_s(charBuff[0], ":", charBuff);
					strBuff = strtok_s(charBuff[0], "}", charBuff);
					y = atoi(strBuff.c_str());
					SetCursorPos(x, y);
					mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
					mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP, x, y, 0, 0);
				}
				if (strBuff == "CLICK_2") { //правый клик
					strtok_s(charBuff[0], ":", charBuff);
					strBuff = strtok_s(charBuff[0], ",", charBuff);
					x = atoi(strBuff.c_str());
					strtok_s(charBuff[0], ":", charBuff);
					strBuff = strtok_s(charBuff[0], "}", charBuff);
					y = atoi(strBuff.c_str());
					SetCursorPos(x, y);
					mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN, x, y, 0, 0);
					mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP, x, y, 0, 0);
				}
			}
			if (strBuff == "keyboard") {
				strtok_s(charBuff[0], "'", charBuff);
				strBuff = strtok_s(charBuff[0], "'", charBuff);
				if (strBuff == "keydown") { //нажатие клавиши
					strtok_s(charBuff[0], "'", charBuff);
					charBuff[1] = charBuff[0];
					strBuff = strtok_s(charBuff[0], "'", charBuff);
					charBuff[2] = charBuff[0];
					strBuff = strtok_s(charBuff[1], " ", charBuff);
					for (i = 0; jskeys[i] != "00"; i++) {
						if (jskeys[i] == strBuff) {
							keybd_event(vkkeys[i], 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
							break;
						}
					}
					strtok_s(charBuff[2], "'", charBuff);
					if (charBuff[0][0] == '\0') {
						continue;
					}
					strBuff = strtok_s(charBuff[0], "'", charBuff);
					if (strBuff == "pressed_time") //иммитация зажатие клавиши
					{	charBuff[0] = strtok_s(charBuff[0], ":", charBuff);
						strBuff = strtok_s(charBuff[0], "}", charBuff);
						for (x = 0; x < atoi(strBuff.c_str()); x++) {
							Sleep(20);
							keybd_event(vkkeys[i], 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
							keybd_event(vkkeys[i], 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
							x = x + 20;
						}
					}
				}
				if (strBuff == "keyup") { //отжатие клавиши
					strtok_s(charBuff[0], "'", charBuff);
					charBuff[1] = charBuff[0];
					strBuff = strtok_s(charBuff[0], "'", charBuff);
					strBuff = strtok_s(charBuff[1], " ", charBuff);
					for (i = 0; jskeys[i] != "00"; i++) {
						if (jskeys[i] == strBuff) {
							keybd_event(vkkeys[i], 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
							break;
						}
					}
				}
			}
		}
	}
	return 0;
}