#include "../../pbcpp/include/pbcpp.cpp"

using namespace pbcpp;

class App {
public:
  App() {
    _tickTimer.subscribe(MINUTE_UNIT, this);
    _window.create()
      .subscribe(this)
      .backgroundColor(GColorBlack)
      .open();
  }

  void onWindowSubscribe(PbWindow::Subscriber<App> & subscriber) {
    subscriber.load();
  }

  void onWindowLoad() {
    const int textHeight = 24 * 5 + 2;
    PbRect bounds = _window.layer().bounds();
    bounds.top((bounds.height() - textHeight) / 2).height(textHeight);
    _textLayer.create(bounds)
      .font(PbFontRef::fromSystem(FONT_KEY_GOTHIC_24_BOLD))
      .backgroundColor(GColorBlack)
      .textColor(GColorWhite)
      .textAlignment(PbSystem::isRound() ? GTextAlignmentCenter : GTextAlignmentLeft);

    refreshTime(PbDateTime::now().localInfo());

    _window.addChild(_textLayer);
  }

  void onTick(const PbDateTimeInfo & dateTimeInfo, TimeUnits unitsChanged) {
    refreshTime(dateTimeInfo);
  }

private:
  void refreshTime(const PbDateTimeInfo & dateTimeInfo) {
    const char * leading = PbSystem::isRound() ? "" : "  ";
    const char * trailing = PbSystem::isRound() ? " " : "";
    _textLayerText.assignFormat("{%s\n%s\"t\": \"", trailing, leading);
    if (PbSystem::is24hStyle()) {
      _textLayerText.appendFormat("%02d:%02d",
        dateTimeInfo.hour(), dateTimeInfo.minute());
    } else {
      _textLayerText.appendFormat("%d:%02d%s",
        dateTimeInfo.hour12(), dateTimeInfo.minute(), dateTimeInfo.isPM() ? "pm" : "am");
    }
    _textLayerText.appendFormat("\",%s\n%s\"k\": \"", trailing, leading)
      .appendDateTimeFormat(9, "%A", dateTimeInfo)
      .appendFormat("\",%s\n%s\"d\": \"", trailing, leading)
      .appendDateTimeFormat(6, "%d %b", dateTimeInfo)
      .appendFormat("\"%s\n}%s", trailing, trailing);
    _textLayer.text(_textLayerText.c_str());
  }

  PbTickTimer _tickTimer;
  PbWindow _window;
  PbTextLayer _textLayer;
  PbString _textLayerText;
};

extern "C" int main() {
  return PbMain<App>::run();
}
