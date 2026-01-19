#pragma once
#include <string>
#include <functional>
#include "../tnl/tnl_util.h"
#include "../tnl/tnl_vector2i.h"
#include "gui/dxe_gui_value_slider.h"
#include "gui/dxe_gui_check_box.h"
#include "gui/dxe_gui_menu_selector.h"


namespace dxe {

	class GraphicsLayer final {
	public:
		GraphicsLayer(const tnl::Vector2i& size);
		GraphicsLayer(const std::string& file_path);
		~GraphicsLayer();

		enum class fAdoption {
			BLOOM	= 1 << 0,
			BLUR	= 1 << 1,
			MONO	= 1 << 2,
			HSB		= 1 << 3,
			LEVEL	= 1 << 4,
		};

		void write(const std::function<void()>& user_write);
		void draw();

		void saveStatus(const std::string& file_path);
		void loadStatus(const std::string& file_path);

		TNL_PROPERTY(int, BlendMode, dx_blend_mode_);
		TNL_PROPERTY(int, BloomThreshold, bloom_threshold_);
		TNL_PROPERTY(int, BloomRatio, bloom_ratio_);
		TNL_PROPERTY(float, BloomAlpha, bloom_alpha_);
		TNL_PROPERTY(int, BlurRatio, blur_ratio_);
		TNL_PROPERTY(float, BlurAlpha, blur_alpha_);

		TNL_PROPERTY(int, MonoCb, mono_cb_);
		TNL_PROPERTY(int, MonoCr, mono_cr_);

		TNL_PROPERTY(int, HsbHue, hsb_hue_);
		TNL_PROPERTY(int, HsbSaturation, hsb_saturation_);
		TNL_PROPERTY(int, HsbBright, hsb_bright_);

		TNL_PROPERTY(int, LevelMin, level_min_);
		TNL_PROPERTY(int, LevelMax, level_max_);
		TNL_PROPERTY(int, LevelGamma, level_gamma_);
		TNL_PROPERTY(int, LevelAfterMin, level_after_min_);
		TNL_PROPERTY(int, LevelAfterMax, level_after_max_);

		TNL_PROPERTY(fAdoption, Adoption, f_adoption_);
		TNL_PROPERTY(tnl::Vector2i, Position, position_);

		TNL_SET_SELF_PROPERTY(bool, UseAlphaChannel, is_enable_alpha_channel_);

		void drawGuiController(const tnl::Vector2i& pos);

	private:
		tnl::Vector2i position_;
		tnl::Vector2i size_;
		int dx_blend_mode_ = DX_BLENDMODE_NOBLEND;

		int scr_col_ = 0;			// 普通の描画結果を書き込むスクリーン
		int scr_heght_bright_ = 0;	// 普通の描画結果から高輝度部分を抜き出した結果を書き込むスクリーン
		int scr_down_scale_ = 0;	// 高輝度部分を縮小した結果を書き込むスクリーン
		int scr_gauss_ = 0;			// 縮小画像をガウスフィルタでぼかした結果を書き込むスクリーン

		int bloom_threshold_ = 230;		// ブルーム用輝度の閾値
		int bloom_ratio_ = 1200;		// ブルーム用のぼかし度合い
		float bloom_alpha_ = 1.0f;		// ブルームフィルタ透明度

		int blur_ratio_ = 1200;			// ブラー用のぼかし度合い
		float blur_alpha_ = 0.0f;		// ブラーフィルタ透明度

		int mono_cb_ = 0;			// モノトーン YCb
		int mono_cr_ = 0;			// モノトーン YCr

		int hsb_hue_ = 0;			// HSB 色相
		int hsb_saturation_ = 0;	// HSB 彩度
		int hsb_bright_ = 0;		// HSB 輝度

		int level_min_ = 0;			// level 補正 変換元の下限値( 0 ～ 255 )
		int level_max_ = 255;		// level 補正 変換元の上限値( 0 ～ 255 )
		int level_gamma_ = 100;		// level 補正 ガンマ値( 100 で 1.0 を表し、ガンマ補正無し、1 より小さい値は不可 )
		int level_after_min_ = 0;	// level 補正 変換後の最低値( 0 ～ 255 )
		int level_after_max_ = 255;	// level 補正 変換後の最大値( 0 ～ 255 )

		fAdoption f_adoption_ = static_cast<fAdoption>(0);
		bool is_enable_alpha_channel_ = false;

		void reset();

		bool is_btn_save_forcus_ = false;
		bool is_btn_save_select_ = false;
		Shared<dxe::GuiCheckBox> gui_use_alpha_channel_;
		Shared<dxe::GuiCheckBox> gui_use_bloom_;
		Shared<dxe::GuiCheckBox> gui_use_blur_;
		Shared<dxe::GuiCheckBox> gui_use_mono_;
		Shared<dxe::GuiCheckBox> gui_use_hsb_;
		Shared<dxe::GuiCheckBox> gui_use_level_;

		Shared<dxe::GuiValueSlider< GraphicsLayer, int >> gui_luminance_threshold_;
		Shared<dxe::GuiValueSlider< GraphicsLayer, float >> gui_filter_bloom_alpha_;
		Shared<dxe::GuiValueSlider< GraphicsLayer, float >> gui_filter_blur_alpha_;
		Shared<dxe::GuiValueSlider< GraphicsLayer, int >> gui_bloom_ratio_;
		Shared<dxe::GuiValueSlider< GraphicsLayer, int >> gui_blur_ratio_;
		Shared<dxe::GuiValueSlider< GraphicsLayer, int >> gui_mono_cb_;
		Shared<dxe::GuiValueSlider< GraphicsLayer, int >> gui_mono_cr_;

		Shared<dxe::GuiValueSlider< GraphicsLayer, int >> gui_hsb_hue_;
		Shared<dxe::GuiValueSlider< GraphicsLayer, int >> gui_hsb_saturation_;
		Shared<dxe::GuiValueSlider< GraphicsLayer, int >> gui_hsb_bright_;

		Shared<dxe::GuiValueSlider< GraphicsLayer, int >> gui_level_min_;
		Shared<dxe::GuiValueSlider< GraphicsLayer, int >> gui_level_max_;
		Shared<dxe::GuiValueSlider< GraphicsLayer, int >> gui_level_gamma_;
		Shared<dxe::GuiValueSlider< GraphicsLayer, int >> gui_level_after_min_;
		Shared<dxe::GuiValueSlider< GraphicsLayer, int >> gui_level_after_max_;

		Shared<dxe::GuiMenuSelector> gui_blend_mode_;

	};

	TNL_ENUM_CLASS_BIT_MASK_OPERATOR(GraphicsLayer::fAdoption);



}
