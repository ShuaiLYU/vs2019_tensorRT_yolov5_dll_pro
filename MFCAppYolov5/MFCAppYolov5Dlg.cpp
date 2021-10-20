
// MFCAppYolov5Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCAppYolov5.h"
#include "MFCAppYolov5Dlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCAppYolov5Dlg 对话框



CMFCAppYolov5Dlg::CMFCAppYolov5Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPYOLOV5_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCAppYolov5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCAppYolov5Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCAppYolov5Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCAppYolov5Dlg 消息处理程序

BOOL CMFCAppYolov5Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	std::string workspace = "F:\\baiduYUN\\tensorRT_yolov5_demo-v1\\workspace\\config.json";
	this->p_yolo = CreateExportObj(workspace);

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCAppYolov5Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCAppYolov5Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCAppYolov5Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

vector<cv::String> fn, fn1;
vector<cv::String> read_images_in_folder(cv::String pattern)
{

	cv::glob(pattern, fn, false);
	//cv::glob(pattern, fn);
	for (int i = 0; i < fn.size(); i++)
	{
		string ImgName = fn[i];
		if (ImgName.find(".bmp") == string::npos && ImgName.find(".tif") == string::npos
			&& ImgName.find(".jpg") == string::npos && ImgName.find(".png") == string::npos)
			continue;
		fn1.push_back(fn[i]);

	}
	return fn1;
	//vector<cv::Mat> images;
	//size_t count = fn.size(); //number of png files in images folder
	//for (size_t i = 0; i < count; i++)
	//{
	//	images.push_back(imread(fn[i]));
	//	imshow("img", imread(fn[i]));
	//
	//}
	//return images;
}

void ShowImage( cv::Mat& img) {
	cv::imshow("OutPutImage", img);
	cv::waitKey(0);
	cv::destroyWindow("OutPutImage");
}






void CMFCAppYolov5Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码


	std:: string img_dir = "F:/baiduYUN/tensorRT_yolov5_demo-v1/workspace/samples/";
	vector<cv::String> image_paths = read_images_in_folder(img_dir);
	vector<cv::Mat> imgs;

	for (int i = 0; i < image_paths.size(); i++)
	{

		cv::String img_path = image_paths.at(i);
		cv::Mat img_gray;
		cv::Mat img_input = cv::Mat::zeros(cv::Size(6400, 600), CV_8UC3);
		cv::Mat img_resize;
		cv::Mat img_raw = cv::imread(img_path);
		cv::resize(img_raw, img_resize, cv::Size(800, 600), 0, 0);
		for (int k = 0; k < 8; k++) {
			img_resize.copyTo(img_input(cv::Rect(k * 800, 0,  800, 600)));
		}
		/*cv::Mat img_resize;
		cv::resize(img_raw, img_resize, cv::Size(512, 512), 0, 0);

		cv::Mat img_input = cv::Mat::zeros(cv::Size(4096, 512), CV_8UC3);
		img_resize.copyTo(img_input(cv::Rect(0, 0, 512, 512)));*/


		imgs.push_back(img_input);
	}
	std::vector<Defects> batch_results;

	//batch_results.resize(imgs.size());

	double time0 = static_cast<double>(cv::getTickCount());
	p_yolo->forward(imgs, batch_results);
	time0 = ((double)cv::getTickCount() - time0) / cv::getTickFrequency();
	cout << "The inference time is ：" << time0 << "s" << endl;

	for (int i = 0; i < imgs.size(); i++)
	{
		Defects* l_defs =& p_yolo->m_defects[i];

		for (int j = 0; j < l_defs->size(); j++)
		{
			cv::Rect rect = l_defs->at(j).rect;
		}
		ShowImage(imgs[i]);

	}
	

}
