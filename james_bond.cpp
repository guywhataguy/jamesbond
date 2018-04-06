/*
    COPYRIGHT (C)
    Code to play a life like version of the "load, defend, shoot game". I really
    don't know a better name for it. Some people on the internet are calling it
   "James Bond" so, we'll roll with that name.
*/
typedef unsigned int uint;

enum class Operation { kShoot = 0, kLoad, kDefend, kNotAvailable };

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
void a_shoot_b_shoot();
void a_shoot_b_load();
void a_load_b_shoot();
void a_shoot_b_defend();
void a_defend_b_shoot();

/* Mutual actions */
void defend();
void nothing();
}  // namespace executer

namespace sensors {
/* this function will poll the sensors simultaneously */
void get_player_moves(Operation* player1_move, Operation* player2_move);
}  // namespace sensors

/* Setup the hardware sensors/motors */
void Setup();
}  // namespace hardware

struct Player {
  uint ammunition;
  uint points;
  Operation current_turn;
};

namespace gamemaster {
Player gA, gB;
uint gTruns;
constexpr uint kMaxTurns = 9;
constexpr uint kMaxScore = 3;
void InitializeGame();
void GetPlayerMoves(Operation* player1_move, Operation* player2_move,
                    uint max_wait_time_miliseconds);
void PlayMove();
/* returns if the game has finished from this turn. Either from points or turn
 * count */
bool RunOneTurn();
void RunOneGame();
void ResetGame();
}  // namespace gamemaster

/* Returns if we should stop the roudns. (we clicked stop for mainenance or
 * something)*/
bool StopRounds();

/* Arduino required functions */

void setup() {
  // put your setup code here, to run once:
  gamemaster::InitializeGame();
}

void loop() {
  while (!StopRounds()) {
    gamemaster::ResetGame();
    gamemaster::RunOneGame();
  }
}
