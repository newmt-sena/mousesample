﻿# include <Siv3D.hpp>

/// @brief アイテムのボタン
/// @param rect ボタンの領域
/// @param texture ボタンの絵文字
/// @param font 文字描画に使うフォント
/// @param name アイテムの名前
/// @param desc アイテムの説明
/// @param count アイテムの所持数
/// @param enabled ボタンを押せるか
/// @return ボタンが押された場合 true, それ以外の場合は false
bool Button(const Rect& rect, const Texture& texture, const Font& font, const String& name, const String& desc, int32 count, bool enabled)
{
	if (enabled)
	{
		rect.draw(ColorF{ 0.3, 0.5, 0.9, 0.8 });

		rect.drawFrame(2, 2, ColorF{ 0.5, 0.7, 1.0 });

		if (rect.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}
	}
	else
	{
		rect.draw(ColorF{ 0.0, 0.4 });

		rect.drawFrame(2, 2, ColorF{ 0.5 });
	}

	texture.scaled(0.5).drawAt(rect.x + 50, rect.y + 50);

	font(name).draw(30, rect.x + 100, rect.y + 15, Palette::White);

	font(desc).draw(18, rect.x + 102, rect.y + 60, Palette::White);

	font(count).draw(50, Arg::rightCenter((rect.x + rect.w - 20), (rect.y + 50)), Palette::White);

	return (enabled && rect.leftClicked());
}

void Main()
{
	// クッキーの絵文字
	const Texture texture{ U"🍣"_emoji };

	// 農場の絵文字
	const Texture farmEmoji{ U"🛥️"_emoji };

	// 工場の絵文字
	const Texture factoryEmoji{ U"🌊"_emoji };

	const Texture feEmoji{ U"💉"_emoji };

	const Texture syoEmoji{ U"🧑‍🍳"_emoji };

	// フォント
	const Font font{ FontMethod::MSDF, 48, Typeface::Bold };

	// クッキーのクリック円
	const Circle cookieCircle{ 170, 300, 100 };

	// クッキーの表示サイズ（倍率）
	double cookieScale = 1.5;

	// クッキーの個数
	double cookies = 0;

	double clickcookies = 1;

	// 農場の所有数
	int32 farmCount = 0;

	// 工場の所有数
	int32 factoryCount = 0;

	int32 feCount = 0;

	int32 syoCount = 0;

	// 農場の価格
	int32 farmCost = 10;

	// 工場の価格
	int32 factoryCost = 100;

	int32 feCost = 500;

	int32 syoCost = 2000;

	// ゲームの経過時間の蓄積
	double accumulatedTime = 0.0;

	while (System::Update())
	{
		// クッキーの毎秒の生産量 (cookies per second) を計算する
		const int32 cps = (farmCount + factoryCount*10+ feCount * 100);

		// ゲームの経過時間を加算する
		accumulatedTime += Scene::DeltaTime();

		// 0.1 秒以上蓄積していたら
		if (0.1 <= accumulatedTime)
		{
			accumulatedTime -= 0.1;

			// 0.1 秒分のクッキー生産を加算する
			cookies += (cps * 0.1);
		}

		// 農場の価格を計算する
		farmCost = 10 + (farmCount * 10);

		// 工場の価格を計算する
		factoryCost = 100 + (factoryCount * 100);

		feCost = 500 + (feCount * 500);

		syoCost = 2000 + (syoCount * 2000);

		// クッキー円上にマウスカーソルがあれば
		if (cookieCircle.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		// クッキー円が左クリックされたら
		if (cookieCircle.leftClicked())
		{
			cookieScale = 1.3;
			cookies=cookies+clickcookies;
		}

		if (KeyZ.down()) {
			cookies = cookies + 500;
		}

		// クッキーの表示サイズを回復する
		cookieScale += Scene::DeltaTime();

		if (1.5 < cookieScale)
		{
			cookieScale = 1.5;
		}

		// 背景を描く
		Rect{ 0, 0, 800, 600 }.draw(Arg::top = ColorF{Palette::Lightblue }, Arg::bottom = ColorF{ Palette:: Royalblue });

		// クッキーの数を整数で表示する
		font(U"{:.0f}"_fmt(cookies)).drawAt(60, 170, 100);

		// クッキーの生産量を表示する
		font(U"毎秒: {}"_fmt(cps)).drawAt(24, 170, 160);

		font(U"クリック: {}"_fmt(clickcookies)).drawAt(24, 170, 190);

		// クッキーを描画する
		texture.scaled(cookieScale).drawAt(cookieCircle.center);

		// 農場ボタン
		if (Button(Rect{ 340, 40, 420, 100 }, farmEmoji, font, U"漁に出る", U"C{} / 1 CPS"_fmt(farmCost), farmCount, (farmCost <= cookies)))
		{
			cookies -= farmCost;
			++farmCount;
		}

		// 工場ボタン
		if (Button(Rect{ 340, 160, 420, 100 }, factoryEmoji, font, U"養殖", U"C{} / 10 CPS"_fmt(factoryCost), factoryCount, (factoryCost <= cookies)))
		{
			cookies -= factoryCost;
			++factoryCount;
		}

		//薬品ボタン
		if (Button(Rect{ 340, 280, 420, 100 }, feEmoji, font, U"クローン作成", U"C{} / 100 CPS"_fmt(feCost), feCount, (feCost <= cookies)))
		{
			cookies -= feCost;
			++feCount;
		}

		if (Button(Rect{ 200, 440, 420, 100 }, syoEmoji, font, U"研修を受ける", U"C{} / 1 click"_fmt(syoCost), syoCount, (syoCost <= cookies)))
		{
			cookies -= syoCost;
			++syoCount;
			++clickcookies;
		}

		
		
	}
}
