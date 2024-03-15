template <typename... Args>
std::string InputScreen::inputArgument(Chart& chart, const std::string& title, Args&... args) {
    std::string pass;
    int count = 0;

    auto get_screen_output = [&](auto& lambda, double& input, const std::string& screen_str, const std::string& range, auto&... args) {
        setScreenString(screen_str);
        if constexpr (sizeof...(args) > 0) {
            lambda(lambda, args...);
        }
    };

    // Lambda for recursive processing
    auto input_all_argument = [&](auto& lambda, std::string& pass, double& input, const std::string& screen_str, const std::string& range, auto&... args) {
        if (pass == "all") {
            double t1 = chart.getNote(chart.getNoteSize() - 1).getValue(NoteAttr::time);
            double t2 = chart.getSpeed(chart.getSpeedSize() - 1).getValue(SpeedAttr::time);
            double t3 = chart.getBPM(chart.getBPMSize() - 1).getValue(BPMAttr::time);
            std::string max_pos = std::to_string(std::max({ t1, t2, t3 }));
            if (max_pos.find('.') != std::string::npos) {
                while (true) {
                    if (max_pos.back() == '0') {
                        max_pos.pop_back();
                    }
                    else if (max_pos.back() == '.'){
                        max_pos.pop_back();
                        break;
                    }
                    else{
                        break;
                    }
                }
            }

            addToStringDeq(max_pos, count);
            input = stod(max_pos);
            count++;
            pass.clear();
        }
        else {
            std::string output = inputFromUser(screen_str, range, count);
            count++;
            pass = output;
            if (output == "x") {
                return;
            }
            else if (output == "all"){
                input = 0;
            }
            else if (isNumber(output)) {
                input = stod(output);
            }
            
        }
        if constexpr (sizeof...(args) > 0) {
            lambda(lambda, pass, args...);
        }
    };

    clearAll();
    setTitle(title);
    get_screen_output(get_screen_output, args...);
    input_all_argument(input_all_argument, pass, args...);
    clearScreen();
    printStringDeq();
    return pass;
}