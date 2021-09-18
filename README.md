# unlimited_int
# Written by Ronen Zyroff in 2019-2021
Arbitrary-precision arithmetic library written in pure C++ for C++. Has similar syntax to int in C++ with operator overloading.
# Compilers
Supports C++14, C++17, C++20\
Supports MSVC++ (the Microsoft C++ compiler)\
Supports GCC's g++ (The GNU Linux compiler)\
Supports clang++\
Supports both 32-bit architectures and 64-bit architectures on GNU and on Windows.\
Can be compiled to a library using the source files inside "unlimited" folder.\
\
Note: There exists an issue in Windows with MinGW GCC / MSYS2 with clang / MSYS2 GCC\
The issue is that thread_local variables cause a crash when multithreading.\
That's why if you're compiling on Windows on MSYS2 and/or on MinGW you just need to not use the function: unlimited::unlimited_int::generate_random_prime_multithreaded() \
Also you can optionally change the macro in unlimited/unlimited_int/structures/settings.hpp UNLIMITED_INT_SUPPORT_MULTITHREADING from true to false.\
Changing the macro that way will cause the problematic function not to be available.\
Even after the "fix", the unlimited library will still crash when used in a multithreaded environment, with those compilers (because of the thread_local bug in those compilers in Windows).
# Reliability
Partially tested, still not quite ready for release yet (as of 18 Septemper 2021)
# Basic use
#include "unlimited.hpp"\
using namespace unlimited;\
int main()\
{\
  unlimited_int small_num(5);\
  unlimited_int big_num("129312981629387124981620398712489112387461298346198234");\
  unlimited_int big_num2("129012873823723432423");\
  unlimited_int result_pow_with_remainder = unlimited_int::pow(small_num, big_num, big_num2);\
  std::cout << "\nResult is: " << result_pow_with_remainder << "\nResult in hex: " << std::hex << result_pow_with_remainder << std::dec;\
  return 0;\
}\
The output of that program will be:\
Result is: 68787405743989542317\
Result in hex: 3ba9df512a7294dad
# Fibonacci example
#include "unlimited.hpp"\
using unlimited::unlimited_int;\
#include \<utility\> //For std::move\
int main()\
{\
  unlimited_int a(1);\
  unlimited_int b(1);\
  bool turn_a = true;\
  for (unlimited_int c(0); c < 20000; ++c)\
  {\
    if (turn_a)\
    {\
      turn_a = false;\
      a += b;\
    }\
    else\
    {\
      turn_a = true;\
      b += a;\
    }\
  }\
  unlimited_int result;\
  if (turn_a)\
    result = std::move(b);\
  else\
    result = std::move(a);\
  std::cout << "\nThe result is: " << result;\
  return 0;\
}\
The output of that program will be:\
The result is: 6626668994574486334215075371113958041694763918460886441085105101387206367373516449587784781431660979378558214235509259430554504652771881084994549935743042119355051752597236695098926399096622003723724466213751922305710983937964593262175486663561357871586137159328489766418338024099429083797008641569963003561152264571189341534062697495866839890665924462034981087087759190868663721431739445641864532113903544589792584989644956521958054497964111696688397510232642353566932946449192359843056091749622646465712930407213455023743299983251635715111412207789329592405941548348949073289468814936732474998181746924799157856455724896651139873008218119990216185028120287952572325228029503258141141138702291283734908035118719851570720732246954281802377221060703910421698339993375541780395369470805838464167640377712102954610997311595872008214606745307129458577879069544055396802264359367565257625925860895298201940215156562423857617691654874153430519895489497082288395513575876375384368210364299746583081989717455748278221270334160379989702380597719658866371511556428196077328270093473943642481847662953637874557246455207273615979265569650706643717101898879649375958179170471644235027904258590945279736394719205615552900623309607666550010814231565716840438340162266929074558425750711095630244824854272601279138541345154444626876993899025177871554041595237286936558153026207585071345259795050300705478817907662211742703224985600043297257727088708657092939021064957903828978311085492397456361028821728471504613781473565788595909583291461429534616629569189565359718515519028188338119050311288503065094662607443810710675920216888082917558824673611790719725432628232927197654068597196242378895044724212857633458800924537197984950524314507770363760243438466014834172743268081129341264632981565128018450987213077305254112870337523646313109478554556185271905954904830408033598638222761390428563236719272421739644114490843810537795478607685479523608483955964445316373778740202476424779381891295493689098452013990736866284312273209274584775228336449806352082307429870316945310768787703870006819923457088992562739715164200721736657744941299434884232303231994110000813082518091908965276126234929983514738682333129044507098781424341693041048049472951356507347492581112551960943633856108653835650635670657602281085739285922362548309022230825660157766147199755024838405114936067594469093436474998951295761200792749240455969993778096292764610394568056746613019262947837054732818840402161963127357496040261956759853765042979898061260129910667114801730034248714374879622340438203226597924844018715003707038688055825416908272294838085578196954694039660270533341844187019459741794434957463173352935156687754890544003770999015472865289505515913264793233742531232871467455216396593433490865400028507377589530083007487420957262932437757081042592560065869374710480619894327659034898168854065048856907208750533846594229520490978460250060252784504653657888204086265261321608645702556350316190260293132251301498747455732799880532246324520684906327734581970239444624626044208141067286740660029324454039302525297187648483647020175245098421860605031174781000684106487718630901722177367466981959477304577955237973753439108451575616078732819787156984256377549511350523234666602617674368790200907906356072076620419139084570553061439970976700547220203349185767525662688041638567275929889247449269654320754686487731867793729046528027567289056571505144329635596141932871102594977199411903235752948471796156036303222777200507320343209069818621598880067585433540073321985989248743421199980853680330392476900124714009885221406529889715149020642360351711765864106550419683218111795955096507859367640630571695754518843948775079623694198061869235772435308688266117040081821648543147064662536819117470990524491118824249067344035380046281079729803694442977762197417252697044844670702852317934370747023376446924299885248380681489191858304139861357399859437847851183770049308290430746523835379495352432434299864120060034470927924216104963969756272346464524086763417183462316331572289434246482138176703079608498639529436502139062279734128311334440602805756004750583309794635391950541934901934998863864325637987222969851022034764071437803751
