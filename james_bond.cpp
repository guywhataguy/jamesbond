/*
    COPYRIGHT (C)
    Code to play a life like version of the "load, defend, shoot game". I really
    don't know a better name for it. Some people on the internet are calling it
   "James Bond" so, we'll roll with that name.

   This code is one file (no .h/.hpp/.cpp) since this is made to run on an
   arduino and therefore have one file will make compiling and uploading more
   convenient.
*/
typedef unsigned int uint;

enum class Operation { kNotAvailable = 0, kLoad, kDefend, kShoot };

namespace hardware {
namespace executer {

/* Breakdown of Hardware Abstraction Logic
 *
 * the possible actions each player can do in one turn are:
 * 1. to shoot
 * 2. to load
 * 3. to defend
 *
 * therefore the possible turns there can be are:
 * player A 	player B
 * ------------------
 * shoot	shoot
 * shoot	load
 * shoot	defend
 * load	shoot
 * load	load
 * load	defend
 * defend	shoot
 * defend	load
 * defend	defend
 *
 * - when one player defends, and the other player loads, the hardware must
 * execute only 'defend'
 * - when both players defend, the hardware must execute
 * only 'defend'
 * - when both players load, the hardware must execute nothing
 *
 * execute something special
 * ------------------
 * shoot	shoot
 * shoot	load
 * load	shoot
 * shoot	defend
 * defend	shoot
 *
 * execute defend
 * ------------------
 * load	defend
 * defend	load
 * defend	defend
 *
 * execute nothing
 * ------------------
 * load	load
 *
 * from here we can see there are really 7 unique move outcomes our hardware
 * has to handle. There wil be a function for each unique move outcome.
 * Inside, each function we will handle the unique timing complexeties
 */

/* Unique situations to handle */
void a_shoot_b_shoot() {
#warning running stub for "a_shoot_b_shoot"
}
void a_shoot_b_load() {
#warning running stub for "a_shoot_b_load"
}
void a_load_b_shoot() {
#warning running stub for "a_load_b_shoot"
}
void a_shoot_b_defend() {
#warning running stub for "a_shoot_b_defend"
}
void a_defend_b_shoot() {
#warning running stub for "a_defend_b_shoot"
}

/* Mutual actions */
void defend() {
#warning running stub for "defend"
}
void nothing() {
#warning running stub for "nothing"
}

/* setup ports for motors */
void Setup() {
#warning running stub for "executer::Setup"
}
}  // namespace executer

namespace sensors {
/* this function will poll the sensors simultaneously */
void get_player_moves(Operation* player1_move, Operation* player2_move) {
#warning running stub for "get_player_moves"
  if (player1_move) {
    *player1_move = Operation::kLoad;
  }
  if (player2_move) {
    *player2_move = Operation::kLoad;
  }
}

/* setup ports for sensors */
void Setup() {
#warning running stub for "sensors::Setup"
}
}  // namespace sensors

namespace userinteraction {
void SetWinnerLeftLight();
void SetWinnerRightLight();
void SetWinnerBothLights();
/* handles the request to reset a game */
void HandleResetButton();
void Setup() {
#warning running stub for "userinteraction::Setup"
}
}  // namespace userinteraction

/* Setup the hardware sensors/motors */
void Setup() {
  sensors::Setup();
  executer::Setup();
  userinteraction::Setup();
}
}  // namespace hardware

struct Player {
  uint ammunition;
  uint points;
  Operation current_turn;
};

namespace gamemaster {
Player gA, gB;
uint gTurns;
constexpr bool kPlayerAOnLeftSide = true;
constexpr bool kPlayerBOnLeftSide = !kPlayerAOnLeftSide;
constexpr uint kMaxTurns = 9;
constexpr uint kMaxScore = 3;
constexpr uint kMilisecWaitForTurn = 800;
void InitializeGame() {
  // initialize hardware, initializing globals will be done in "ResetGame"
  hardware::Setup();
  gamemaster::ResetGame();
}
void GetPlayerMoves(Operation* player1_move, Operation* player2_move,
                    uint max_wait_time_miliseconds) {
  hardware::sensors::get_player_moves(player1_move, player2_move);
}
void PlayMove(Player* a, Player* b);

/* returns if the game ended this turn */
bool RunOneTurn() {
  GetPlayerMoves(&gA.current_turn, &gB.current_turn, kMilisecWaitForTurn);
  PlayMove(&gA, &gB);
  return GameIsDone();
}
/* returns if the game ended this turn */
bool GameIsDone() {
  return (gTurns >= kMaxTurns) || (gA.points >= kMaxScore) ||
         (gB.points >= kMaxScore);
}
bool PlayerWon(const Player& p) { return (p.points >= kMaxScore); }

void RunOneGame() {
  while (!gamemaster::RunOneTurn()) {
  }
  // now annoucing winner
  bool status_player_a = PlayerWon(gA);
  bool status_player_b = PlayerWon(gB);
  if (status_player_a == status_player_b) {
    // either both won or both lost
    hardware::userinteraction::SetWinnerBothLight();
    return;
  }
  if (status_player_a) {
    // player a won
    if (kPlayerAOnLeftSide) {
      hardware::userinteraction::SetWinnerLeftLight();
      return;
    } else {
      hardware::userinteraction::SetWinnerRightLight();
      return;
    }
  } else {
    // player b won
    if (kPlayerBOnLeftSide) {
      hardware::userinteraction::SetWinnerLeftLight();
      return;
    } else {
      hardware::userinteraction::SetWinnerRightLight();
      return;
    }
  }
}
void ResetGame() {
  // reset globals
  gTurns = 0;
  gA.ammunition = 0;
  gA.current_turn = Operation::kNotAvailable;
  gA.points = 0;
  gB.ammunition = 0;
  gB.current_turn = Operation::kNotAvailable;
  gB.points = 0;
}
}  // namespace gamemaster

/* Arduino required functions */

void setup() {
  // put your setup code here, to run once:
  gamemaster::InitializeGame();
}

void loop() {
  gamemaster::ResetGame();
  gamemaster::RunOneGame();
}
