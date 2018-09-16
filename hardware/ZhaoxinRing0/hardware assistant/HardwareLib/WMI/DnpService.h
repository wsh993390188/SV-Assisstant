// http://www.dinop.com/vc/service_ctrl.html (ja)

#pragma once

#include <winsvc.h>
#include "atlstr.h"

class	CDnpService
{
	//
	//	サービスの起動／停止用スレッドクラス
	//
	class CServiceThread
	{
	public:
		CServiceThread();

	private:

		bool					_bCancel;			//サービスの起動／停止処理中断用変数、trueなら中断開始
		CComAutoCriticalSection	_secbCancel;		//サービスの起動／停止処理中断用クリティカルセクション

	public:

		//
		//	サービスの起動／停止処理中断用関数
		//
		//	中断したい場合はIsCancel(true,true)を呼び出す
		//
		bool	IsCancel(bool bSave = false, bool bNewValue = false);



		//
		//	サービスの簡易コントロール
		//
		//	そのまま呼び出すとサービスが起動／停止するまで無限ループで待機する。
		//	スレッド中で呼び出し、IsCancel()を利用することで無限ループに陥らない
		//	コントロールが可能。
		//
		bool EasyStartStop(LPCTSTR pszName, bool b);
		
	};


public:


	//
	//	サービスの簡易コントロール
	//
	//	サービスが起動/停止するまで無限ループで待機する。
	//
	bool	EasyStartStop(LPCTSTR pszName, bool bStart);


	//
	//	サービスの簡易起動
	//
	//	サービスが起動するまで無限ループで待機する。
	//
	bool	EasyStart(LPCTSTR pszName);


	//
	//	サービスの簡易停止
	//
	//	サービスが停止するまで無限ループで待機する。
	//
	bool	EasyStop(LPCTSTR pszName);


	//
	//	サービスの簡易再起動
	//
	//	サービスが再起動するまで無限ループで待機する。
	//
	bool	EasyRestart(LPCTSTR pszName);



	//
	//	指定するサービスが動いているかのチェック
	//
	//	falseの場合は"停止"とは限らない。サービスが存在しない場合などもfalseとなる。
	//
	bool	IsServiceRunning(LPCTSTR pszName);
	
};
