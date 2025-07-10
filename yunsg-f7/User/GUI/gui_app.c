#include "gui_app.h"
#include "lvgl.h"
//#include "message_send.h"
#include "stdio.h"

/*获取当前活动屏幕的宽高*/
#define scr_act_width() lv_obj_get_width(lv_scr_act())
#define scr_act_height() lv_obj_get_height(lv_scr_act())

static lv_obj_t *textarea; /*文本区域*/

static lv_point_t points[] = {{0, 0}, {0, 200}}; /*分割线坐标*/

/* 按钮矩阵数组 */
static const char *num_map[] = {"1", "2", "3", "\n",
                                "4", "5", "6", "\n",
                                "7", "8", "9", "\n",
                                "*", "0", "#", ""};

/**
 * @brief  按钮矩阵事件回调
 * @param  *e ：事件相关参数的集合，它包含了该事件的所有数据
 * @return 无
 */
// static void btnm_event_cb(lv_event_t *e)
// {
//     uint8_t id;

//     lv_event_code_t code = lv_event_get_code(e); /* 获取事件类型 */
//     lv_obj_t *target = lv_event_get_target(e);   /* 获取触发源 */

//     const char *txt = lv_textarea_get_text(textarea);

//     if (code == LV_EVENT_VALUE_CHANGED)
//     {
//         id = lv_btnmatrix_get_selected_btn(target); /* 获取按键索引 */
//         VoiceJson_t json;

//         if (id == 9)
//         {
//             lv_textarea_del_char(textarea); /*删除文本区域的一个字符*/
//             strcpy(json.topic, "voice");
//             strcpy(json.msg.option, "button");
//             strcpy(json.msg.value, "*");
//             add_message_to_send_queue(&json);
//         }
//         else if (id == 11)
//         {
//             /*数据处理*/
//             const char *txt = lv_textarea_get_text(textarea);
//             printf("判断是否存在 %s\n", txt);
//             strcpy(json.topic, "voice");
//             strcpy(json.msg.option, "button");
//             strcpy(json.msg.value, "#");
//             add_message_to_send_queue(&json);
//             lv_textarea_set_text(textarea,"");
//         }
//         else
//         {

//             if (strlen(txt) < 6)
//             {
//                 const char *current_txt = lv_btnmatrix_get_btn_text(target, id); /*文本区域增加按钮对应的文本*/
//                 VoiceJson_t json;
//                 strcpy(json.topic, "voice");
//                 strcpy(json.msg.option, "button");
//                 strcpy(json.msg.value, current_txt);
//                 add_message_to_send_queue(&json);
//                 lv_textarea_add_text(textarea, current_txt); /* 添加按钮文本到文本区域 */
//             }
//         }
//     }
// }

void gui_app(void)
{
    /*欢迎标签*/
    lv_obj_t *label_wel = lv_label_create(lv_scr_act());
    lv_label_set_text(label_wel, "Welcome");
    lv_obj_set_style_text_color(label_wel, lv_color_hex(0x000000), LV_PART_MAIN); /*设置文本颜色*/
    lv_obj_set_style_text_font(label_wel, &lv_font_montserrat_24, LV_PART_MAIN);  /*设置文本字体*/
    lv_obj_set_style_text_align(label_wel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);   /*文本居中对齐*/
    lv_obj_align(label_wel, LV_ALIGN_TOP_MID, 0, 20);                             /*设置位置*/

    /*取件码标签*/
    lv_obj_t *label_pickUpCode = lv_label_create(lv_scr_act());
    lv_label_set_text(label_pickUpCode, "Pick up code");
    lv_obj_set_style_text_color(label_pickUpCode, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_text_font(label_pickUpCode, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_align(label_pickUpCode, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_align(label_pickUpCode, LV_ALIGN_CENTER, -200, -100);

    /*文本区域*/
    textarea = lv_textarea_create(lv_scr_act());
    lv_textarea_set_max_length(textarea, 6);                                     /*限制输入文本为6位*/
    lv_textarea_set_cursor_pos(textarea, 0);                                     /*定义光标位置*/
    lv_obj_align_to(textarea, label_pickUpCode, LV_ALIGN_OUT_BOTTOM_MID, 0, 30); /*设置位置*/
    lv_textarea_set_one_line(textarea, true);                                    /*单行输入*/

    /* 分隔线 */
    lv_obj_t *line = lv_line_create(lv_scr_act());                /* 创建线条 */
    lv_line_set_points(line, points, 2);                          /* 设置线条坐标点 */
    lv_obj_align(line, LV_ALIGN_CENTER, 0, 0);                    /* 设置位置 */
    lv_obj_set_style_line_color(line, lv_color_hex(0xcdcdcd), 0); /* 设置线条颜色 */

    /* 按钮矩阵（创建） */
    lv_obj_t *btnm = lv_btnmatrix_create(lv_scr_act());                      /* 创建按钮矩阵 */
    lv_obj_set_size(btnm, scr_act_width() * 2 / 5, scr_act_width() * 2 / 5); /* 设置大小 */
    lv_btnmatrix_set_map(btnm, num_map);                                     /* 设置按钮 */
    lv_obj_align(btnm, LV_ALIGN_RIGHT_MID, -scr_act_width() / 16, 0);        /* 设置位置 */
    lv_obj_set_style_text_font(btnm, &lv_font_montserrat_24, LV_PART_ITEMS); /* 设置字体 */

    /* 按钮矩阵（优化界面） */
    lv_obj_set_style_border_width(btnm, 0, LV_PART_MAIN);                   /* 去除主体边框 */
    lv_obj_set_style_bg_opa(btnm, 0, LV_PART_MAIN);                         /* 设置主体背景透明度 */
    lv_obj_set_style_bg_opa(btnm, 0, LV_PART_ITEMS);                        /* 设置按钮背景透明度 */
    lv_obj_set_style_shadow_width(btnm, 0, LV_PART_ITEMS);                  /* 去除按钮阴影 */
    //lv_obj_add_event_cb(btnm, btnm_event_cb, LV_EVENT_VALUE_CHANGED, NULL); /* 设置按钮矩阵回调 */
}
