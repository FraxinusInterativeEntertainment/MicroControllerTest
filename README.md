# 简介

这是一个模板文档

# 接口

获取当天有效场次
[GET] /game_map/get_available_game_sessions/
返回：
{'err_code': 0, 'err_msg': '', 'game_sessions_info': [{'game_id': 1, 'game_time': '2019-10-11 12:00:00', 'status': 'p'}, ...]}

获取本场游戏参与玩家列表接口
[GET] /game_map/get_current_game_group_user/
返回：
{'err_code': 0, 'err_msg': '', 'assess_user_list': [{'id': 1, 'nickname': 'tom', 'avatar': '123'}, ...]}

本场游戏用户评价接口
[POST] /game_map/evaluate_user/
参数：assessed_user_id（被评价用户的id），thumb_up（1：点赞，0：不点赞，默认0）
返回：
{'err_code': 0, 'err_msg': ''}

获取wiki group对应的信息
[GET] /game_map/get_wiki_group_info/
参数：wiki_group_id
返回：
{'err_code': 0, 'err_msg': '', 'wiki_group_info': {'ID': '123', 'Title':'123', 'Image': '123', 'Description': '123'}}

获取用户已解锁的wiki group列表
[GET] /game_map/get_available_wiki_group_list/
参数：wiki_type
返回：
{'err_code': 0, 'err_msg': '', 'wiki_group_names':['123','345',...]}

# 更新

Version 0.0.1: 建立了模板
