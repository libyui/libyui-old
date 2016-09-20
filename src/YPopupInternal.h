
#include <string>
#include <vector>

// Internal helper class for YDialogSpy and YPropertyEditor
class YPopupInternal
{
public:

    /**
     * Display a simple popup dialog with OK button
     * @param label the message to display
     */
    static void message(const std::string &label);

    typedef std::vector<std::string> StringArray;

    /**
     * Display a popup dialog with several input fields
     * @param  array fields to edit
     * @param  label title of the dialog
     * @return true if dialog was closed by [OK], false otherwise
     */
    static bool editStringArray(StringArray &array, const std::string &label);

    /**
     * Display a popup dialog with 3 initially empty input fields
     * @param  label title of the dialog
     * @return Entered values in a StringArray, if canceled the array is empty.
     */
    static StringArray editNewStringArray(const std::string &label);
};
