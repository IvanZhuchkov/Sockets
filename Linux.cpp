#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
extern "C"{
#include <xdo.h>
}
using namespace std;

int config() { // ������� ������ ���� ������������
	string strBuff;
	char configBuff[1024];
	FILE* config = new FILE;
	config = fopen("config.txt", "r");
	fgets(configBuff, 1024, config);
	strtok(configBuff, " ");                                           
	strBuff = strtok(NULL, "");
	return atoi(strBuff.c_str());
}

int main() {
	//��������� ��� ������ - ��� ������� � ������� ������� �� ����� java script � ����������� ���������� XWindow
	string jskeys[] = { "Escape","F1","F2","F3","F4","F5","F6", "F7", "F8", "F9", "F10", "F11", "F12", "PrintScreen","Delete","Home","PageUp","PageDown","End","Backquote","Digit1","Digit2", "Digit3", "Digit4", "Digit5", "Digit6", "Digit7", "Digit8", "Digit9", "Digit0", "Minus","Equal","Backspace","NumLock","NumpadDivide","NumpadMultiply","NumpadSubtract","Tab","KeyQ","KeyW","KeyE","KeyR","KeyT","KeyY","KeyU","KeyI","KeyO","KeyP","BracketLeft","BracketRight","Backslash","Numpad7","Numpad8","Numpad9","NumpadAdd","CapsLock","KeyA","KeyS","KeyD","KeyF","KeyG","KeyH","KeyJ","KeyK","KeyL","Semicolon","Quote","Enter","Numpad4","Numpad5","Numpad6","ShiftLeft","KeyZ","KeyX","KeyC","KeyV","KeyB","KeyN","KeyM","Comma","Period","Slash","ShiftRight","Numpad1","Numpad2","Numpad3","NumpadEnter","ControlLeft","MetaLeft","AltLeft","Space","AltRight","ContextMenu","ControlRight","ArrowLeft","ArrowUp","ArrowDown","ArrowRight","Numpad0","NumpadDecimal","00"};
	string xkeys[] = { "escape","F1","F2","F3","F4","F5","F6", "F7", "F8", "F9", "F10", "F11", "F12", "PrintScreen","delete","home","Page_Up","Page_Down","end","quoteleft","1","2", "3", "4", "5", "6", "7", "8", "9", "0", "minus","equal","Backspace","Num_Lock","divide","multiply","subtract","Tab","q","w","e","r","t","y","u","i","o","p","bracketleft","bracketright","backslash","7","8","9","add","Caps_Lock","a","s","d","f","g","h","j","k","l","semicolon","quote","Return","4","5","6","Shift_L","z","x","c","v","b","n","m","comma","period","slash","Shift_R","1","2","3","Return","Control_L","Meta_L","Alt_L","space","Alt_R","ContextMenu","Control_R","Left","Up","Down","Right","0","decimal","00" };
	int i = 0,x=0,y=0;
	double e,f;
	int erStat =0;
	string strBuff;
	sockaddr_in servInfo;
	in_addr ip_to_num;
	xdo_t *Dis = xdo_new(NULL);
	sockaddr_in clientInfo;
	int clientInfo_size=0;
	const short BUFF_SIZE = 1024;
	char servBuff[BUFF_SIZE];
	char configBuff[BUFF_SIZE];
	char** charBuff = new char*[3];
	for (i = 0; i < 3; i++) {
		charBuff[i] = new char[20];
	}
	short packet_size = 0;
	int port;
	int ServSock = socket(AF_INET, SOCK_STREAM, 0);//�������� ���������� ������
	port = config();
	erStat = inet_pton( AF_INET,"127.0.0.1",&ip_to_num); //������� ip �� ��������� �������
	if (erStat <= 0) {
		cout << "Error in IP translation to special numeric format" << endl;
		return 1;
	}
	servInfo.sin_family = AF_INET;//������ ������ � ������ � sockaddr_in
	servInfo.sin_addr = ip_to_num;
	servInfo.sin_port = htons(port);
	erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo)); //�������� ���������� ������ � ip � �����
	if (erStat != 0) {
		cout << "Error Socket binding to server info. Error # " << erStat << endl;
		close(ServSock);
		return 1;
	}
	else {
		cout << "Binding socket to Server info is OK" << endl;
	}
		erStat = listen(ServSock, SOMAXCONN);
		if (erStat != 0) {
			cout << "Can't start to listen to. Error # " << erStat << endl;
			close(ServSock);
			return 1;
		}
		else {
			cout << "Listening..." << endl;
		}
		clientInfo_size = sizeof(clientInfo);
		int ClientConn = accept(ServSock, NULL, NULL); //����������� ������� � �������
		if (ClientConn < 0) {
			cout << "Client detected, but can't connect to a client. Error # " << erStat << endl;
			close(ServSock);
			close(ClientConn);
			return 1;
		}
		else {
			cout << "Connection to a client established successfully" << endl;
		}
		while (1) { // ������ ����� ��������� ������ �� �������
			packet_size = recv(ClientConn, servBuff, BUFF_SIZE, 0);
			if (packet_size == 0) {
				continue;
			}
			//��������� 4 ������ - ���������� �������, ���� �� �������� ��������� exit
			if ((servBuff[0] == 'e') && (servBuff[1] == 'x') && (servBuff[2] == 'i') && (servBuff[3] == 't')) {
				shutdown(ClientConn, SHUT_RDWR);
				close(ClientConn);
				close(ServSock);
				break;
			}
			cout << "Client's message:" << " " << servBuff << endl;
			//������ ���������� ���������� ������
			strtok(servBuff, "'");
			strBuff = strtok(NULL, "'");
			if (strBuff == "MOUSE") {
				strtok(NULL, "'");
				strBuff = strtok(NULL, "'");
				if (strBuff == "MOVE_CURSOR") {//������������ ����
					strtok(NULL, ":");
					strBuff = strtok(NULL, ",");
					x = atoi(strBuff.c_str());
					strtok(NULL, ":");
					strBuff = strtok(charBuff[0], "}");
					y = atoi(strBuff.c_str());
					xdo_move_mouse(Dis, x, y, 0);
				}
				if (strBuff == "CLICK_1") { //����� ����
					strtok(NULL, ":");
					strBuff = strtok(NULL, ",");
					x = atoi(strBuff.c_str());
					strtok(NULL, ":");
					strBuff = strtok(NULL, "}");
					y = atoi(strBuff.c_str());
					xdo_move_mouse(Dis, x, y, 0);
					xdo_click_window(Dis, CURRENTWINDOW, 1);
				}
				if (strBuff == "CLICK_2") {//������ ����
					strtok(NULL, ":");
					strBuff = strtok(NULL, ",");
					x = atoi(strBuff.c_str());
					strtok(NULL, ":");
					strBuff = strtok(NULL, "}");
					y = atoi(strBuff.c_str());
					xdo_move_mouse(Dis, x, y, 0);
					xdo_click_window(Dis, CURRENTWINDOW, 2);
				}
			}
			if (strBuff == "keyboard") {
				strtok(NULL, "'");
				strBuff = strtok(NULL, "'");
				if (strBuff == "keydown") {//������� �������
					strtok(NULL, "'");
					charBuff[0] = strtok(NULL, "'");
					charBuff[1]=strtok(NULL, "'");
					e=0;
					if (charBuff[1][0] != '}') {
						strBuff = strtok(NULL, "'");
						if(strBuff == "pressed_time"){
							strtok(NULL,":");
							strBuff=strtok(NULL,"}");
							e=atoi(strBuff.c_str())/1000.0;
						}
					}
					charBuff[2]=charBuff[0];
					strBuff = strtok(charBuff[0], " ");
					for (i = 0; jskeys[i] != "00"; i++) {
						if (jskeys[i] == strBuff) {
							xdo_send_keysequence_window_down(Dis, CURRENTWINDOW, xkeys[i].c_str(), 0);
						}
					}
					if (e!=0) // ��������� ������� �������
					{	sleep(1);
						for (f = 0; f < e; f+=0.1) {
							sleep(0.1);
						}
					}
				}
				if (strBuff == "keyup") {//������� �������
					strtok(NULL, "'");
					charBuff[0] = strtok(NULL, "'");
					strBuff = strtok(charBuff[0], " ");
					for (i = 0; jskeys[i] != "00"; i++) {
						if (jskeys[i] == strBuff) {
							xdo_send_keysequence_window_up(Dis, CURRENTWINDOW, xkeys[i].c_str(), 0);
							break;
						}
					}
				}
			}
		}
	return 0;
}

