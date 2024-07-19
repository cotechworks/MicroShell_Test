#include "microshell.h"
#include "mscmd.h"
#include "msconf.h"
static MICROSHELL ms;
static MSCMD mscmd;

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}

/* ============================== MicroShell =============================== */
static MSCMD_USER_RESULT usrcmd_system(MSOPT *msopt, MSCMD_USER_OBJECT usrobj);
static MSCMD_USER_RESULT usrcmd_help(MSOPT *msopt, MSCMD_USER_OBJECT usrobj);

//コマンドテーブル
static MSCMD_COMMAND_TABLE table[] = {
  { "system", usrcmd_system },
  { "help", usrcmd_help },
};

//systemコマンド処理
static MSCMD_USER_RESULT usrcmd_system(MSOPT *msopt, MSCMD_USER_OBJECT usrobj) {
  Serial.println("[SYSTEM]");

  int argc;
  msopt_get_argc(msopt, &argc);
  for (int i = 0; i < argc; ++i) {
    char buf[MSCONF_MAX_INPUT_LENGTH];
    msopt_get_argv(msopt, i, buf, sizeof(buf));
    Serial.println(buf);
  }

  return 0;
}

//helpコマンド処理
static MSCMD_USER_RESULT usrcmd_help(MSOPT *msopt, MSCMD_USER_OBJECT usrobj) {
  Serial.print(
    "system : system command\r\n"
    "help   : help command\r\n");

  return 0;
}

///////////////////////////////////////////////////////////////////////////////////

// 送信用関数の紐づけ
static void utx(char c) {
  Serial.write(&c, 1);
}

//受信用関数の紐づけ
static char urx(void) {
  char c;
  while (!Serial.available()) {
  }
  Serial.readBytes(&c, 1);
  return c;
}

static void action_hook(MSCORE_ACTION action) {
}

//MicroShellの初期化
void setup1() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();

  microshell_init(&ms, utx, urx, action_hook);
  mscmd_init(&mscmd, table, std::extent<decltype(table)>::value, nullptr);
}

//MicroShellのメインループ
void loop1() {
  char buf[MSCONF_MAX_INPUT_LENGTH];
  Serial.print("MicroShell>");
  microshell_getline(&ms, buf, sizeof(buf));
  MSCMD_USER_RESULT r;
  mscmd_execute(&mscmd, buf, &r);
}