#if defined _config_included
	#endinput
#endif

#define _config_included

#define function%0(%1)					forward %0(%1); public %0(%1)

#define AIRDROP_STATUS_INVALID			0
#define AIRDROP_STATUS_LAUNCH			1
#define AIRDROP_STATUS_START			2
#define AIRDROP_STATUS_CREATE			3
#define AIRDROP_STATUS_RUN				4
#define AIRDROP_STATUS_DROP				5
#define AIRDROP_STATUS_PICKEDUP			6

#define AIRDROP_INSTALLABLE_Z			50.0// высота самолета от земли
#define AIRPLANE_DISTANCE_DEPARTURE		400.0// расстояние старта самолета от края гз
#define AIRPLANE_DISTANCE_ARRIVAL		800.0// расстояние остановки самолета от места старта(удаление)
#define AIRPLANE_SPEED					25.0// скаорость движения самолета(для увелчения скорости, увеличить цифру)
#define PARACHUTE_SPEED					10.0// скорость падения сумки с парашютом(для увелчения скорости, увеличить цифру)
#define START_AIRDROP_TIMER				(1000*60)// время, через которое будет запущено событие. 1000*60 - это 1 минута. Далее делаем рандом на своё усмотрение. Далее смотрите запуск таймера в o_ad_ActionAirDrop(status), статус AIRDROP_STATUS_LAUNCH
#define AIRDROP_COLOR					0x02F89EFF
#define MAFIA_COLOR						0x8C8C8CFF
#define AIRDROP_FORMAT_COLOR			"{02F89E}"
#define MAFIA_FORMAT_COLOR				"{8C8C8C}"
#define AIRDROP_MAFIA_PRIZE_MONEY		200000
#define AIRDROP_MAFIA_PRIZE_RAITING		1500
#define AIRDROP_MAFIA_PRIZE_MATERIAL	25000
#define AIRDROP_PLAYER_PRIZE_MONEY		100000
#define AIRDROP_PLAYER_PRIZE_RAITING	1500
// --- эти данный не нужны, т.к. в каждом моде свои переменные, содержащие данную информацию. Используются исключительно для теста работы системы ---
#define MAX_MEMBER						4
// --- подправьте эти макросы под себя
#define GetPlayerOrg(%0)			GetPVarInt(%0,"ad_member")// выаши переменные содержащие данный игрока о принадлежности к организации
#define GivePlayerRating(%0,%1)		SetPVarInt(%0,"ad_rating",GetPVarInt(%0,"ad_rating")+%1)// выаши переменные содержащие данный игрока о его рейтинге
#define GivePlayerMoneyEx(%0,%1)	SetPVarInt(%0,"ad_money",GetPVarInt(%0,"ad_money")+%1)// выаши переменные содержащие данный игрока о его деньгах
#define GiveMemberMoney(%0,%1)		MI[%0][miMoney] += %1// выаши переменные содержащие данный фракции о деньгах
#define GiveMemberRating(%0,%1)		MI[%0][miRating] += %1// выаши переменные содержащие данный фракции о рейтинге
#define GiveMemberMaterial(%0,%1)	MI[%0][miMaterial] += %1// выаши переменные содержащие данный фракции о материалах
// --- 