# 简介

这是一个模板文档

# 接口

**获取当天有效场次**  
[GET] /game_map/*****/  
返回：  
{'err_code': 0, 'err_msg': '', 'game_info': [{'game_id': 1, 'game_time': '2019-10-11 12:00:00', 'status': 'p'}, ...]}  

**获取本场游戏参与玩家列表接口**  
[GET] /game_map/******/  
返回：  
{'err_code': 0, 'err_msg': '', 'assess_list': [{'id': 1, 'nickname': 'tom', 'avatar': '123'}, ...]}  


# 更新

Version 0.0.1: 建立了模板
