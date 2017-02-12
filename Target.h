class Target;

typedef std::function<void(Target*, WiFiClient client)> tdCallback;
class Target
{
  public:

    Target(String caption, String subtitle, String request, String Id,
            const unsigned int onCode,  const unsigned int offCode, const tdCallback& process)
    {
      this->_caption = caption;
      this->_request =  request;

      this->_onCode =  onCode;
      this->_offCode =  offCode;

      this->_process = process;

      this->_state = 0;
      this->_mode = 0;
      this->_id = Id;
      this->_subtitle = subtitle;


    };
    unsigned int _onCode;
    unsigned int _offCode;
    String _caption;
    String _subtitle;
    String _request;
    String _id;

    int _mode; // 0 = off, 1 = on
    int _state; // 0 = off, 1 = on
    tdCallback _process;


    String toString()
    {
      String smode = _mode == 1 ? "/on" : "/off";
      return _caption + "=" + _request + smode;
    }






};
