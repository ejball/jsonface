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
    PbRect bounds = _window.layer().bounds();
    bounds.top(bounds.top() + 16).height(bounds.height() - 32);
    _textLayer.create(bounds)
      .font(PbFontRef::fromSystem(FONT_KEY_GOTHIC_24_BOLD))
      .backgroundColor(GColorBlack)
      .textColor(GColorWhite)
      .textAlignment(GTextAlignmentLeft);

    refreshTime(PbDateTime::now().localInfo());

    _window.addChild(_textLayer);
  }

  void onTick(const PbDateTimeInfo & dateTimeInfo, TimeUnits unitsChanged) {
    refreshTime(dateTimeInfo);
  }

private:
  void refreshTime(const PbDateTimeInfo & dateTimeInfo) {
    _textLayerText.assign("{\n  \"t\": \"");
    if (clock_is_24h_style()) {
      _textLayerText.appendFormat("%02d:%02d",
        dateTimeInfo.hour(), dateTimeInfo.minute());
    } else {
      _textLayerText.appendFormat("%d:%02d%s",
        dateTimeInfo.hour12(), dateTimeInfo.minute(), dateTimeInfo.isPM() ? "pm" : "am");
    }
    _textLayerText.append("\",\n  \"k\": \"")
      .appendDateTimeFormat(9, "%A", dateTimeInfo)
      .append("\",\n  \"d\": \"")
      .appendDateTimeFormat(6, "%d %b", dateTimeInfo)
      .append("\"\n}");
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
