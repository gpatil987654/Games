#include <Windows.h>
#include <cstdlib>

#define ID_NUMBER 509
#define ID_NUMBER2 500
#define ScorebarHeight 50

HBRUSH hGreenBrush, hBlackBrush, hOrangeBrush;
RECT rect, prect, rrect, rprect, circlerect, pcirclerect;
int xcorner, ycorner, xclient, yclient, ping, bping, bbping, firstflag, start, Auto, Aflag, level, quit = 1;
int flag, newsize = 0, rud, lud, Player1Score, Player2Score, Timer, randomnumber, randomnumber1;

LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI Thread1(LPVOID);

void BallDraw(HWND, LPRECT, LPRECT, int);

void PlayerpadLeft(HWND, LPRECT, LPRECT, int, int, int);

void PlayerpadRight(HWND, LPRECT, LPRECT, int, int, int);

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR CmdLine, int iCmdShow)
{
	HWND hwnd;
	WNDCLASSEX wndclass;
	TCHAR ClassName[] = TEXT("MyClass");
	MSG msg;

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = ClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.cbClsExtra = NULL;
	wndclass.cbWndExtra = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(ClassName, TEXT("PING-PONG"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, SW_MAXIMIZE);

	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return ((int)msg.wParam);
}

LRESULT WINAPI WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int x, y, control;
	HDC hdc;
	PAINTSTRUCT ps;

	HANDLE hThread1 = NULL;

	TCHAR str[125];
	TCHAR str2[] = TEXT("Controls:\nA - : For Single Palyer Mode\nM - : For Multiplayer Mode\nR - : To start / Reset Game\nSpace - : To pause / resume game\nUp-Arraow - : To move right gamepad upwards\nDown-Arrow - : To move right gamepad downwards\nW - : To move left gamepad upwards in Multiplayer mode\nS- : To move left gamepad downwards in Multiplayer mode\nI - : For game controls ");

	TCHAR str3[] = TEXT("This Game contains Two modes.\nOne is Single - Player and Second is Multiplayer.\nDefault one is Single Plyaer Mode.\nIn Single Player Mode There are 3 Levels and need to score 3-Points before computer to complete each level.");
	switch (iMsg)
	{
	case WM_CREATE:
		hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread1, (LPVOID)hwnd, 0, NULL);

		control = 1;

		rect.left = 0;
		rect.top = 0;
		rect.right = 20;
		rect.bottom = 100;
		xcorner = 12;
		ycorner = 12;

		prect = rect;

		rrect.left = 1100;
		rrect.top = 0;
		rrect.right = 1120;
		rrect.bottom = 100;

		rprect = rrect;

		circlerect.left = 100;
		circlerect.top = 100;
		circlerect.right = 120;
		circlerect.bottom = 120;

		pcirclerect = circlerect;

		ping = 0;
		bping = 0;
		firstflag = 1;
		Player1Score = 0;
		Player2Score = 0;
		Aflag = 0;
		level = 1;
		Auto = 1;
		randomnumber = 5;

		start = 1;
		break;

	case WM_SIZE:
		xclient = LOWORD(lParam);
		yclient = HIWORD(lParam);

		rrect.left = xclient - 20;
		rrect.right = xclient;

		rprect.left = xclient - 20;
		rprect.right = xclient;

		newsize = 1;

		if (firstflag == 0)
		{
			PlayerpadLeft(hwnd, &rect, &prect, flag, xclient, yclient);

			PlayerpadRight(hwnd, &rrect, &rprect, flag, xclient, yclient);
		}

		break;

	case WM_TIMER:

		Timer = 1;

		KillTimer(hwnd, ID_NUMBER);
		Timer = 0;
		flag = 2;

		if (circlerect.right >= rrect.left - 3 && circlerect.top <= rrect.bottom && circlerect.bottom >= rrect.top)
		{
			ping = 1;
			PlaySound(TEXT("sound2.wav"), NULL, SND_FILENAME | SND_ASYNC);
			if (rud == 0)
			{
				circlerect.bottom -= randomnumber;
				circlerect.top -= randomnumber;
				circlerect.left -= randomnumber;
				circlerect.right -= randomnumber;
				bbping = 0;
			}

			if (rud == 1)
			{
				circlerect.bottom += randomnumber;
				circlerect.top += randomnumber;
				circlerect.left -= randomnumber;
				circlerect.right -= randomnumber;
				bbping = 1;
			}

			bping = 1;
		}

		if (circlerect.left <= rect.right && circlerect.top <= rect.bottom && circlerect.bottom >= rect.top)
		{
			ping = 0;
			PlaySound(TEXT("sound2.wav"), NULL, SND_FILENAME | SND_ASYNC);
			if (lud == 0)
			{
				circlerect.bottom -= randomnumber;
				circlerect.top -= randomnumber;
				circlerect.left += randomnumber;
				circlerect.right += randomnumber;
				bbping = 0;
			}

			if (lud == 1)
			{
				circlerect.bottom += randomnumber;
				circlerect.top += randomnumber;
				circlerect.left += randomnumber;
				circlerect.right += randomnumber;
				bbping = 1;
			}

			bping = 0;
		}

		if (circlerect.bottom >= yclient)
		{
			ping = 2;
		}

		if (circlerect.top <= (ScorebarHeight + 5))
		{
			ping = 3;
		}

		if (ping == 0)
		{

			if (bbping == 0)
			{
				circlerect.bottom -= randomnumber;
				circlerect.top -= randomnumber;
				circlerect.left += randomnumber;
				circlerect.right += randomnumber;
			}

			if (bbping == 1)
			{
				circlerect.bottom += randomnumber;
				circlerect.top += randomnumber;
				circlerect.left += randomnumber;
				circlerect.right += randomnumber;
			}
		}
		if (ping == 1)
		{

			if (bbping == 0)
			{
				circlerect.bottom -= randomnumber;
				circlerect.top -= randomnumber;
				circlerect.left -= randomnumber;
				circlerect.right -= randomnumber;
			}

			if (bbping == 1)
			{
				circlerect.bottom += randomnumber;
				circlerect.top += randomnumber;
				circlerect.left -= randomnumber;
				circlerect.right -= randomnumber;
			}
		}

		if (ping == 2)
		{
			if (bping == 1)
			{
				circlerect.bottom -= randomnumber;
				circlerect.top -= randomnumber;
				circlerect.left -= randomnumber;
				circlerect.right -= randomnumber;
			}

			if (bping == 0)
			{
				circlerect.bottom -= randomnumber;
				circlerect.top -= randomnumber;
				circlerect.left += randomnumber;
				circlerect.right += randomnumber;
			}
		}

		if (ping == 3)
		{
			if (bping == 1)
			{
				circlerect.bottom += randomnumber;
				circlerect.top += randomnumber;
				circlerect.left -= randomnumber;
				circlerect.right -= randomnumber;
			}

			if (bping == 0)
			{
				circlerect.bottom += randomnumber;
				circlerect.top += randomnumber;
				circlerect.left += randomnumber;
				circlerect.right += randomnumber;
			}
		}

		BallDraw(hwnd, &circlerect, &pcirclerect, flag);

		if (circlerect.right >= (rrect.right - 5) || circlerect.left <= (rect.left + 5))
		{
			ping = -1;

			if (circlerect.right >= (rrect.right - 5))
			{

				Player1Score += 1;
				InvalidateRect(hwnd, NULL, FALSE);
			}

			if (circlerect.left <= (rect.left + 5))
			{

				InvalidateRect(hwnd, NULL, FALSE);
				Player2Score += 1;
			}

			if (Auto == 1 && Player2Score == 3)
			{
				if (level == 3)
				{

					wsprintf(str, TEXT("You Have Win The Game!!! Congrats \n Press R to restart the Game"));

					MessageBox(hwnd, str, TEXT("WINNER"), MB_OK | MB_ICONINFORMATION);
				}
				else
				{

					wsprintf(str, TEXT("You passed the level-%d\nPress R for next level"), level);

					MessageBox(hwnd, str, TEXT("LEVEL PASSED"), MB_OK);
				}
			}
			else if (Auto == 1 && Player1Score == 3)
			{
				wsprintf(str, TEXT("You lost at level-%d\nPress R to play again"), level);

				MessageBox(hwnd, str, TEXT("LEVEL Failed"), MB_OK);
			}
			break;
		}
		else
			SetTimer(hwnd, ID_NUMBER, 15, NULL);

		break;

	case WM_CHAR:
		switch (wParam)
		{

		case 'i':

			MessageBox(hwnd, str2, TEXT("Cantrols"), MB_OK | MB_ICONINFORMATION);
			break;

		case 'r':
			// RESET
			if (ping == -1)
			{
				ping = 0;
				bping = 0;
				circlerect.left = xclient / 2;
				circlerect.top = yclient / 2;
				circlerect.right = (xclient / 2 + 20);
				circlerect.bottom = (yclient / 2 + 20);

				pcirclerect.left = xclient / 2;
				pcirclerect.top = yclient / 2;
				pcirclerect.right = (xclient / 2 + 20);
				pcirclerect.bottom = (yclient / 2 + 20);
			}

			firstflag = 1;

			if (Auto == 1)
			{
				if ((Player2Score == 3) && (level < 3))
				{

					level++;
					Player1Score = 0;
					Player2Score = 0;
					randomnumber += 2;
				}
				else if (Player1Score == 3)
				{
					level = 1;

					Player1Score = 0;
					Player2Score = 0;
					randomnumber = 5;
				}
			}
			if (Auto == 0)
			{
				randomnumber = 6;
			}

			InvalidateRect(hwnd, NULL, TRUE);

			SetTimer(hwnd, ID_NUMBER, 50, NULL);

			break;

		case 'a':

			if (Auto == 0)
			{

				firstflag = 1;
				Player1Score = 0;
				Player2Score = 0;

				KillTimer(hwnd, ID_NUMBER);

				ping = 0;
				bping = 0;
				circlerect.left = xclient / 2;
				circlerect.top = yclient / 2;
				circlerect.right = (xclient / 2 + 20);
				circlerect.bottom = (yclient / 2 + 20);

				pcirclerect.left = xclient / 2;
				pcirclerect.top = yclient / 2;
				pcirclerect.right = (xclient / 2 + 20);
				pcirclerect.bottom = (yclient / 2 + 20);
				InvalidateRect(hwnd, NULL, TRUE);
			}
			Auto = 1;
			break;

		case 'm':

			if (Auto == 1)
			{

				level = 1;
				firstflag = 1;
				Player1Score = 0;
				Player2Score = 0;

				KillTimer(hwnd, ID_NUMBER);

				ping = 0;
				bping = 0;
				circlerect.left = xclient / 2;
				circlerect.top = yclient / 2;
				circlerect.right = (xclient / 2 + 20);
				circlerect.bottom = (yclient / 2 + 20);

				pcirclerect.left = xclient / 2;
				pcirclerect.top = yclient / 2;
				pcirclerect.right = (xclient / 2 + 20);
				pcirclerect.bottom = (yclient / 2 + 20);

				InvalidateRect(hwnd, NULL, TRUE);
			}
			Auto = 0;
			break;

		case 's':

			if (Auto == 0)
			{
				rect.top += 15;
				rect.bottom += 15;
				lud = 1;
				flag = 0;
				PlayerpadLeft(hwnd, &rect, &prect, flag, xclient, yclient);
			}
			break;

		case 'w':
			if (Auto == 0)
			{
				rect.top -= 15;
				rect.bottom -= 15;
				lud = 0;
				flag = 0;

				PlayerpadLeft(hwnd, &rect, &prect, flag, xclient, yclient);
			}

			break;
		}

		break;

	case WM_KEYDOWN:

		switch (wParam)
		{

		case VK_SPACE:
			// START RESUME PAUSE
			if (start == 0)
			{
				KillTimer(hwnd, ID_NUMBER);
				start = 1;
			}
			else if (start == 1)
			{
				SetTimer(hwnd, ID_NUMBER, 50, NULL);
				start = 0;
			}

			break;

		case VK_UP:

			rrect.top -= 20;
			rrect.bottom -= 20;
			flag = 1;
			rud = 0;
			PlayerpadRight(hwnd, &rrect, &rprect, flag, xclient, yclient);

			break;

		case VK_DOWN:

			rrect.top += 20;
			rrect.bottom += 20;
			flag = 1;
			rud = 1;

			PlayerpadRight(hwnd, &rrect, &rprect, flag, xclient, yclient);

			break;
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
		hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
		hOrangeBrush = CreateSolidBrush(RGB(255, 128, 0));

		SelectObject(hdc, hOrangeBrush);
		Rectangle(hdc, 0, 0, xclient, ScorebarHeight);

		SetBkColor(hdc, RGB(255, 128, 0));
		SetTextColor(hdc, RGB(0, 0, 255));

		if (Auto == 1)
		{
			wsprintf(str, TEXT("COMPUTER SCORE:%d"), Player1Score);

			TextOut(hdc, 0, 10, str, lstrlen(str));

			wsprintf(str, TEXT("SINGLE PLAYER : LEVEL-%d"), level);

			TextOut(hdc, xclient / 2 - 60, 10, str, lstrlen(str));

			wsprintf(str, TEXT("PLAYER  SCORE:%d"), Player2Score);
			TextOut(hdc, xclient - 150, 10, str, lstrlen(str));
		}
		if (Auto == 0)
		{
			wsprintf(str, TEXT("PLAYER 1 SCORE:%d"), Player1Score);

			TextOut(hdc, 0, 10, str, lstrlen(str));

			wsprintf(str, TEXT("MULTI  PLAYER"));

			TextOut(hdc, xclient / 2 - 50, 10, str, lstrlen(str));

			wsprintf(str, TEXT("PLAYER 2 SCORE:%d"), Player2Score);
			TextOut(hdc, xclient - 150, 10, str, lstrlen(str));
		}

		if (firstflag == 1)
		{
			rrect.top = yclient / 2 - 50;
			rrect.bottom = yclient / 2 + 50;
			rprect = rrect;

			SelectObject(hdc, hBlackBrush);
			RoundRect(hdc, rprect.left, rprect.top, rprect.right, rprect.bottom, xcorner, ycorner);
			DeleteObject(hBlackBrush);

			SelectObject(hdc, hGreenBrush);
			RoundRect(hdc, rrect.left, rrect.top, rrect.right, rrect.bottom, xcorner, ycorner);
			DeleteObject(hGreenBrush);

			rect.top = yclient / 2 - 50;
			rect.bottom = yclient / 2 + 50;
			prect = rect;

			SelectObject(hdc, hBlackBrush);
			RoundRect(hdc, prect.left, prect.top, prect.right, prect.bottom, xcorner, ycorner);
			DeleteObject(hBlackBrush);

			SelectObject(hdc, hGreenBrush);
			RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, xcorner, ycorner);
			DeleteObject(hGreenBrush);

			firstflag = 0;
		}

		if (control == 1)
		{

			MessageBox(hwnd, str3, TEXT("Game Info"), MB_OK | MB_ICONINFORMATION);

			MessageBox(hwnd, str2, TEXT("Game Cantrols"), MB_OK | MB_ICONINFORMATION);
			control = 0;
		}
		EndPaint(hwnd, &ps);

		break;

	case WM_DESTROY:
		quit = 0;
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

DWORD WINAPI Thread1(LPVOID param)
{
	HDC hdc;
	MSG msg;
	TCHAR str[125];
	static long i = 0, j = 0;

	while (quit == 1)
	{
		if ((Auto == 1) && (Timer == 1))
		{

			if (rect.bottom >= yclient)
				Aflag = 1;
			if ((rect.bottom <= yclient) && (Aflag == 0))
			{

				if ((circlerect.left - 50) <= rect.right && level == 1)
				{
					if (circlerect.top < (rect.top))
					{
						rect.top -= 10;
						rect.bottom -= 10;
						lud = 0;
					}
					else if (circlerect.top > (rect.bottom))
					{
						rect.top += 10;
						rect.bottom += 10;
						lud = 1;
					}
				}
				else if ((circlerect.left - 100) <= rect.right && level == 2)
				{
					if (circlerect.top < (rect.top))
					{
						rect.top -= 15;
						rect.bottom -= 15;
						lud = 0;
					}
					else
					{
						rect.top += 15;
						rect.bottom += 15;
						lud = 1;
					}
				}
				else if ((circlerect.left - 120) <= rect.right && level == 3)
				{
					if (circlerect.top < (rect.top))
					{
						rect.top -= 20;
						rect.bottom -= 20;
						lud = 0;
					}
					else
					{
						rect.top += 20;
						rect.bottom += 20;
						lud = 1;
					}
				}
				else
				{
					rect.top += 5;
					rect.bottom += 5;
					lud = 1;
					flag = 0;
				}
				PlayerpadLeft((HWND)param, &rect, &prect, flag, xclient, yclient);
			}
			if (rect.top <= ScorebarHeight)
				Aflag = 0;

			if ((rect.top >= ScorebarHeight) && (Aflag == 1))
			{
				if ((circlerect.left - 50) <= rect.right && level == 1)
				{
					if (circlerect.top < (rect.top))
					{
						rect.top -= 10;
						rect.bottom -= 10;
						lud = 0;
					}
					else
					{
						rect.top += 10;
						rect.bottom += 10;
						lud = 1;
					}
				}
				else if ((circlerect.left - 100) <= rect.right && level == 2)
				{

					if (circlerect.top < (rect.top))
					{
						rect.top -= 15;
						rect.bottom -= 15;
						lud = 0;
					}
					else
					{
						rect.top += 15;
						rect.bottom += 15;
						lud = 1;
					}
				}
				else if ((circlerect.left - 120) <= rect.right && level == 3)
				{

					if (circlerect.top < (rect.top))
					{
						rect.top -= 20;
						rect.bottom -= 20;
						lud = 0;
					}
					else
					{
						rect.top += 20;
						rect.bottom += 20;
						lud = 1;
					}
				}
				else
				{
					rect.top -= 5;
					rect.bottom -= 5;
					lud = 0;
					flag = 0;
				}
				PlayerpadLeft((HWND)param, &rect, &prect, flag, xclient, yclient);
			}
		}
	}

	return 0;
}

void PlayerpadRight(HWND hwnd, LPRECT rect, LPRECT prect, int flag, int xclient, int yclient)
{

	HDC hdc;
	HBRUSH hBlackBrush, hGreenBrush;
	int xcorner = 12, ycorner = 12;
	hdc = GetDC(hwnd);

	hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));

	SelectObject(hdc, hBlackBrush);

	if (prect->top >= ScorebarHeight && prect->bottom <= yclient)
		RoundRect(hdc, prect->left, prect->top, prect->right, prect->bottom, xcorner, ycorner);

	else if (prect->top <= ScorebarHeight)
		RoundRect(hdc, prect->left, ScorebarHeight, prect->right, ScorebarHeight + 100, xcorner, ycorner);

	else if (prect->bottom >= yclient)
		RoundRect(hdc, prect->left, yclient - 100, prect->right, yclient, xcorner, ycorner);

	SelectObject(hdc, hGreenBrush);

	if (rect->top >= ScorebarHeight && rect->bottom <= yclient)
		RoundRect(hdc, rect->left, rect->top, rect->right, rect->bottom, xcorner, ycorner);

	else if (rect->top <= ScorebarHeight)
		RoundRect(hdc, rect->left, ScorebarHeight, rect->right, ScorebarHeight + 100, xcorner, ycorner);

	else if (rect->bottom >= yclient)
		RoundRect(hdc, rect->left, yclient - 100, rect->right, yclient, xcorner, ycorner);

	DeleteObject(hBlackBrush);
	DeleteObject(hGreenBrush);

	prect->left = rect->left;
	prect->top = rect->top;
	prect->right = rect->right;
	prect->bottom = rect->bottom;

	ReleaseDC(hwnd, hdc);
}

void PlayerpadLeft(HWND hwnd, LPRECT rect, LPRECT prect, int flag, int xclient, int yclient)
{

	HDC hdc;
	HBRUSH hBlackBrush, hGreenBrush;
	int xcorner = 12, ycorner = 12;
	hdc = GetDC(hwnd);

	hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));

	SelectObject(hdc, hBlackBrush);

	if (prect->top >= ScorebarHeight && prect->bottom <= yclient)
		RoundRect(hdc, prect->left, prect->top, prect->right, prect->bottom, xcorner, ycorner);

	else if (prect->top <= ScorebarHeight)
		RoundRect(hdc, prect->left, ScorebarHeight, prect->right, ScorebarHeight + 100, xcorner, ycorner);

	else if (prect->bottom >= yclient)
		RoundRect(hdc, prect->left, yclient - 100, prect->right, yclient, xcorner, ycorner);

	SelectObject(hdc, hGreenBrush);

	if (rect->top >= ScorebarHeight && rect->bottom <= yclient)
		RoundRect(hdc, rect->left, rect->top, rect->right, rect->bottom, xcorner, ycorner);

	else if (rect->top <= ScorebarHeight)
		RoundRect(hdc, rect->left, ScorebarHeight, rect->right, ScorebarHeight + 100, xcorner, ycorner);

	else if (rect->bottom >= yclient)
		RoundRect(hdc, rect->left, yclient - 100, rect->right, yclient, xcorner, ycorner);

	prect->left = rect->left;
	prect->top = rect->top;
	prect->right = rect->right;
	prect->bottom = rect->bottom;

	DeleteObject(hGreenBrush);

	DeleteObject(hBlackBrush);

	ReleaseDC(hwnd, hdc);
}

void BallDraw(HWND hwnd, LPRECT rect, LPRECT prect, int flag)
{

	HDC hdc;
	HBRUSH hBlackBrush, hGreenBrush;
	hdc = GetDC(hwnd);

	hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));

	if (flag == 2)
	{

		SelectObject(hdc, hBlackBrush);

		Ellipse(hdc, prect->left, prect->top, prect->right, prect->bottom);

		SelectObject(hdc, hGreenBrush);

		Ellipse(hdc, rect->left, rect->top, rect->right, rect->bottom);
		// DeleteObject(hGreenBrush);
		prect->left = rect->left;
		prect->top = rect->top;
		prect->right = rect->right;
		prect->bottom = rect->bottom;
	}

	ReleaseDC(hwnd, hdc);
	DeleteObject(hBlackBrush);
	DeleteObject(hGreenBrush);
}
