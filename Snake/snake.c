#include <Windows.h>
#include <math.h>

#define ID_TIME 123

typedef struct snake snake_t;

// Linked-list structure Declration
struct snake
{
	RECT rect;			// RECT is Structure contains left,right,top and bottom arguments for rectangle
	struct snake *next; // Slef referencila pointer to maintain link-list property
};

// Functions Declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawSnake(HWND hwnd, int dir, snake_t *, RECT);
void DrawMouse(HWND, int, int, int);

int iSnakeLength = 1;
static snake_t *s1 = NULL;

BOOL isEqual(RECT rect1, RECT rect2)
{
	if (rect1.left == rect2.left && rect1.right == rect2.right && rect1.top == rect2.top && rect1.bottom == rect2.bottom)
	{
		return TRUE;
	}
	else
		return FALSE;
}

// Entry-Point Function like main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	WNDCLASSEX wndclass; // WindowClass is structure which stores attributes of the Application window
	HWND hwnd;			 // Unique Window Handle
	MSG msg;			 // MSG is structure which contains attirbutes which stores message information (mesasage is like any operation that user performs on application window)
	TCHAR ClassName[] = TEXT("MyClass2");
	HBRUSH hBR = CreateSolidBrush(RGB(255, 125, 255));

	// Window Class Structure Initilaisation
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.cbSize = sizeof(wndclass);
	wndclass.lpszClassName = ClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	// Register Window Class
	RegisterClassEx(&wndclass);

	// creates Application-Window in Memory
	hwnd = CreateWindow(
		ClassName,			 // Window-class Structure Name to use
		TEXT("Snake2"),		 // Name of Application-Window in Caption Bar of Application
		WS_OVERLAPPEDWINDOW, // Window-Application style
		CW_USEDEFAULT,		 // below for parameters gives default width,height and top left corner position of the application
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	// Show Window On Screen
	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);

	// Create Link List by initialising head of link list
	s1 = (snake_t *)malloc(sizeof(snake_t));
	(s1->rect).left = 400;
	(s1->rect).right = 440;
	(s1->rect).top = 100;
	(s1->rect).bottom = 140;

	s1->next = NULL;

	// Message Loop to Take User Inputs
	// Whenever user presses any mouse-button or keyboard key this functions translates the user provided input and give to
	// the Window Procedure Function which acts according to the message handler.
	// This Loops terminates when user press cross button on top right corner or when press Alt+f4 or when press on close option from system menu
	while (GetMessage(&msg, NULL, 0, 0)) // Getmessage get message in msg structure from message pool
	{
		TranslateMessage(&msg); // trnslated the hardware message
		DispatchMessage(&msg);	// dispatches the message to window-procedure function
	}

	return (int)msg.wParam;
}

// window-Procedure Structure
// the functions parameters are same as the MSG structure attributes
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	HPEN hpenRed, hpenBlack;
	HBRUSH hbrushgreen, hbrushPink;
	static int Snakedir, xclient, yclient;

	static RECT prect, rrect, rect;

	snake_t *s2 = NULL;
	snake_t *s3 = NULL;

	static int Ex, Ey, mousedied;
	static int SelfEat, resume;

	switch (iMsg)
	{
	case WM_SIZE:

		xclient = LOWORD(lParam);
		yclient = HIWORD(lParam);

		break;

		// Every 100 miliseconds this message is recived
		//
	case WM_TIMER:
		KillTimer(hwnd, ID_TIME);

		// Mouse
		// Eating

		if ((s1->rect.left <= Ex && s1->rect.top <= Ey && s1->rect.bottom >= (Ey + 20) && s1->rect.right >= (Ex + 20) && mousedied == 0))
		{

			mousedied = 1;
			iSnakeLength++;

			s2 = s1;

			while (s2->next != NULL)
			{
				s2 = s2->next;
			}

			if (Snakedir == 1)
			{
				rect.bottom = s2->rect.top;
				rect.top = rect.bottom - 40;
			}
			else if (Snakedir == 3)
			{

				rect.top = s2->rect.bottom;
				rect.bottom = rect.top + 40;
			}
			else if (Snakedir == 2)
			{

				rect.left = s2->rect.right;
				rect.right = rect.left + 40;
			}
			else if (Snakedir == 4)
			{
				rect.right = s2->rect.left;
				rect.left = rect.right - 40;
			}
			//	s3 = new Csnake(rect);

			s3 = (snake_t *)malloc(sizeof(snake_t));
			s3->rect = rect;

			s2->next = s3;

			s3 = NULL;
			s2 = NULL;
		}

		// mouse
		if (mousedied == 1)
		{

			// DrawMouse(hwnd, Ex, Ey, mousedied);

		label_1:
			Ex = (rand() % 20) * 40;

			Ey = (rand() % 20) * 40;

			if (s1->next != NULL)
			{
				s2 = s1->next;
				while (s2->next != NULL)
				{
					if (Ex == s2->rect.left && Ey == s2->rect.top)
					{

						goto label_1;
					}
					s2 = s2->next;
				}
				s2 = NULL;
			}

			mousedied = 0;
		}
		DrawMouse(hwnd, Ex, Ey, mousedied);

		// snake

		if (Snakedir == 1)
		{
			rect.bottom = s1->rect.top;
			rect.top = rect.bottom - 40;
		}
		else if (Snakedir == 3)
		{

			rect.top = s1->rect.bottom;
			rect.bottom = rect.top + 40;
		}
		else if (Snakedir == 2)
		{

			rect.left = s1->rect.right;
			rect.right = rect.left + 40;
		}
		else if (Snakedir == 4)
		{
			rect.right = s1->rect.left;
			rect.left = rect.right - 40;
		}

		if (s1->next != NULL)
		{
			s2 = s1->next;
			while (s2->next != NULL)
			{
				if (isEqual(rect, s2->rect))
				{
					// MessageBox(hwnd, TEXT("HI"), TEXT("Hi"), MB_OK);
					SelfEat = 1;
				}
				s2 = s2->next;
			}
		}

		if (SelfEat == 0)
		{

			/*
			if (rect.left <= 0)
			{
				rect.left = xclient;
				rect.right = xclient + 40;
			}
			*/

			s2 = (snake_t *)malloc(sizeof(snake_t));
			s2->rect = rect;

			s2->next = s1;
			s1 = s2;

			while (s2->next->next != NULL)
			{
				s2 = s2->next;
			}

			prect = s2->next->rect;
			s2->next = NULL;

			rect = s1->rect;

			if (s1->rect.left < -20)
			{
				s1->rect.left = 1560;
				s1->rect.right = s1->rect.left + 40;
			}

			else if (s1->rect.right > 1560)
			{
				s1->rect.left = 0;
				s1->rect.right = s1->rect.left + 40;
			}

			else if (s1->rect.top < -20)
			{
				s1->rect.top = 800;
				s1->rect.bottom = s1->rect.top + 40;
			}
			else if (s1->rect.bottom > 800)
			{
				s1->rect.top = 0;
				s1->rect.bottom = s1->rect.top + 40;
			}
			DrawSnake(hwnd, Snakedir, s1, prect);

			SetTimer(hwnd, ID_TIME, 200, NULL);
		}
		else
		{
			TCHAR str[128];
			wsprintf(str, "Snake Died!!! Your Score Is %d", iSnakeLength);
			MessageBox(hwnd, str, TEXT("The End"), MB_OK | MB_ICONEXCLAMATION);
			PostQuitMessage(0);
		}
		break;

	case WM_CREATE:

		rect.left = 0;
		rect.right = 40;
		rect.top = 0;
		rect.bottom = 40;

		Snakedir = 2;
		mousedied = 1;
		SelfEat = 0;
		resume = 1;
		// SetTimer(hwnd, ID_TIME, 400, NULL);

		break;

		// WM_PAINT draws on Window
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hbrushPink = CreateSolidBrush(RGB(0, 0, 0)); // CreateSolidBrush(RGB(255, 125, 255));
		RECT plainrect = rect;
		SelectObject(hdc, hbrushPink);

		for (int i = 0; i < (xclient); i = i + 40)
		{
			for (int j = 0; j < yclient; j = j + 40)
			{
				RoundRect(hdc, plainrect.left, plainrect.top, plainrect.right, plainrect.bottom, 10, 10);
				plainrect.left = i;
				plainrect.right = i + 40;
				plainrect.top = j;
				plainrect.bottom = j + 40;
			}
		}
		EndPaint(hwnd, &ps);

		break;

		// User-Input Handlers for keypresses
	case WM_KEYDOWN:
		switch (wParam)
		{

			// press space-bar to start the application

		case VK_SPACE:

			if (resume == 1)
			{
				resume = 0;
				SetTimer(hwnd, ID_TIME, 100, NULL); // set the timer for 100 milisecond and after compeltion of 100 mili-second the WM_TIMER message send to the window-Application
			}
			else if (resume == 0)
			{

				KillTimer(hwnd, ID_TIME);
				resume = 1;
			}

			break;

		// Press up-Arrow Key to move snake Upwards
		case 'w':
		case 'W':
		case VK_UP:

			if (Snakedir == 2 || Snakedir == 4)
			{
				KillTimer(hwnd, ID_TIME);
				// rect.bottom = s1->rect.top;
				// rect.top = rect.bottom - 40;
				Snakedir = 1;

				SetTimer(hwnd, ID_TIME, 100, NULL);
			}

			break;

		// Press Down-Arrow Key to Move snake Downwards
		case 's':
		case 'S':
		case VK_DOWN:
			if (Snakedir == 2 || Snakedir == 4)
			{
				KillTimer(hwnd, ID_TIME);
				// rect.top = s1->rect.bottom;
				// rect.bottom = rect.top + 40;
				Snakedir = 3;

				SetTimer(hwnd, ID_TIME, 100, NULL);
			}
			break;

			// Press Right-Arrow Key To Move Snake to Right side
		case 'd':
		case 'D':
		case VK_RIGHT:
			if (Snakedir == 1 || Snakedir == 3)
			{
				KillTimer(hwnd, ID_TIME);
				// rect.left = s1->rect.right;
				// rect.right = rect.left + 40;
				Snakedir = 2;

				SetTimer(hwnd, ID_TIME, 100, NULL);
			}

			break;

		// Press Left-Arrow Key To Move Snake To Left Side
		case 'a':
		case 'A':
		case VK_LEFT:
			if (Snakedir == 1 || Snakedir == 3)
			{
				KillTimer(hwnd, ID_TIME);
				// rect.right = s1->rect.left;
				// rect.left = rect.right - 40;
				Snakedir = 4;

				SetTimer(hwnd, ID_TIME, 100, NULL);
			}

			break;
		}

		break;

	case WM_DESTROY:
		// DeleteObject(hpenRed);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

// Function to draw snake
void DrawSnake(HWND hwnd, int dir, snake_t *s1, RECT prect)
{
	HDC hdc;
	HBRUSH hgreenbrush, hblackbrush, hyellowbrush;

	// POINT p[50];
	hdc = GetDC(hwnd);
	//	Csnake *s2 = new Csnake(s1->rect);
	snake_t *s2 = NULL;
	s2 = (snake_t *)malloc(sizeof(snake_t));
	s2->rect = s1->rect;

	s2->next = s1->next;

	hgreenbrush = CreateSolidBrush(RGB(0, 255, 0));

	hyellowbrush = CreateSolidBrush(RGB(190, 125, 0));

	SelectObject(hdc, hyellowbrush);
	RoundRect(hdc, s2->rect.left, s2->rect.top, s2->rect.right, s2->rect.bottom, 10, 10);

	SelectObject(hdc, hgreenbrush);

	s2 = s2->next;

	for (int i = 1; i < iSnakeLength; i++)
	{

		RoundRect(hdc, s2->rect.left, s2->rect.top, s2->rect.right, s2->rect.bottom, 10, 10);

		s2 = s2->next;
	}
	hblackbrush = CreateSolidBrush(RGB(0, 0, 0));

	SelectObject(hdc, hblackbrush);
	RoundRect(hdc, prect.left, prect.top, prect.right, prect.bottom, 10, 10);

	DeleteObject(hblackbrush);
	DeleteObject(hgreenbrush);
	DeleteObject(hyellowbrush);

	ReleaseDC(hwnd, hdc);
}

// function to draw mouse randomly
void DrawMouse(HWND hwnd, int x, int y, int m)
{

	HDC hdc;
	HBRUSH hredbrush, hblackbrush, hyellowbrush;

	hdc = GetDC(hwnd);

	hredbrush = CreateSolidBrush(RGB(255, 0, 0));
	hblackbrush = CreateSolidBrush(RGB(0, 0, 0));

	SelectObject(hdc, hredbrush);

	if (m == 0)
	{

		SelectObject(hdc, hredbrush);

		Ellipse(hdc, x, y, x + 40, y + 40);
	}
	else if (m == 1)
	{
		SelectObject(hdc, hblackbrush);

		Ellipse(hdc, x, y, x + 40, y + 40);
	}

	ReleaseDC(hwnd, hdc);
}
