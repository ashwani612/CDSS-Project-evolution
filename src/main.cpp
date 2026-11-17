#include <drogon/drogon.h>
#include <sqlite3.h>
#include <iostream>
#include <cctype>
#include <functional>
#include <sstream>
#include <string>
using namespace drogon;
using Callback = std::function<void(const HttpResponsePtr &)>;

sqlite3 *database = nullptr;

// Escape text before showing it inside HTML.
std::string escapeHtml(const std::string &text)
{
    std::string out;
    for (char ch : text)
    {
        switch (ch)
        {
        case '&': out += "&amp;"; break;
        case '<': out += "&lt;"; break;
        case '>': out += "&gt;"; break;
        case '"': out += "&quot;"; break;
        case '\'': out += "&#39;"; break;
        default: out += ch; break;
        }
    }
    return out;
}

// Send an HTML page to the browser.
void sendPage(const Callback &callback, const std::string &title,
              const std::string &content)
{
    std::string page = R"(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>)" + escapeHtml(title) + R"(</title>
<style>
* { box-sizing: border-box; }
body { margin: 0; font-family: Arial, sans-serif; background: #f2f6fa; color: #243447; }
header { background: #145374; color: white; padding: 22px; text-align: center; }
nav { background: #00334e; padding: 13px; text-align: center; }
nav a { color: white; margin: 8px 12px; text-decoration: none; font-weight: bold; }
main { max-width: 1100px; margin: 25px auto; padding: 22px; background: white; border-radius: 10px; }
h2 { color: #145374; }
.card { display: inline-block; vertical-align: top; width: 30%; min-width: 220px; margin: 1%; padding: 20px; background: #eaf4f8; border-radius: 8px; }
.button, button { display: inline-block; background: #087e8b; color: white; border: 0; border-radius: 5px; padding: 11px 17px; text-decoration: none; cursor: pointer; }
label { display: block; margin-top: 13px; font-weight: bold; }
input, textarea, select { width: 100%; padding: 11px; margin-top: 6px; border: 1px solid #b8c7d1; border-radius: 5px; }
textarea { min-height: 80px; }
table { width: 100%; border-collapse: collapse; margin-top: 18px; }
th, td { border: 1px solid #cbd5dc; padding: 10px; text-align: left; overflow-wrap: anywhere; }
th { background: #145374; color: white; }
tr:nth-child(even) { background: #f1f6f8; }
.notice { padding: 12px; background: #e4f6e9; border-left: 4px solid #249653; margin: 15px 0; }
.warning { padding: 12px; background: #fff5df; border-left: 4px solid #d49a20; margin-top: 20px; }
footer { text-align: center; padding: 20px; color: #52616b; }
@media(max-width:700px) { .card { width: 100%; margin: 8px 0; } nav a { display: inline-block; } main { margin: 10px; } }
</style>
</head>
<body>
<header><h1>Clinical Decision Support System</h1><p>Evolution 3 – C++ Web Application</p></header>
<nav>
<a href="/">Home</a>
<a href="/register">Patient Registration</a>
<a href="/history">Patient History</a>
<a href="/monitoring">Health Monitoring</a>
<a href="/analysis">Clinical Analysis</a>
</nav>
<main>)" + content + R"(</main>
<footer>CDSS College Project – Educational Demonstration</footer>
</body>
</html>)";

    auto response = HttpResponse::newHttpResponse();
    response->setContentTypeCode(CT_TEXT_HTML);
    response->setBody(page);
    callback(response);
}

// Run a SQL statement that does not return rows.
bool runSql(const std::string &sql)
{
    char *errorMessage = nullptr;
    int result = sqlite3_exec(database, sql.c_str(), nullptr, nullptr, &errorMessage);
    if (errorMessage)
        sqlite3_free(errorMessage);
    return result == SQLITE_OK;
}

// Save patient registration details safely using SQL parameters.
bool savePatient(const std::string &name, const std::string &age,
                 const std::string &gender, const std::string &phone,
                 const std::string &symptoms)
{
    const char *sql = "INSERT INTO patients(name, age, gender, phone, symptoms) VALUES(?,?,?,?,?)";
    sqlite3_stmt *statement = nullptr;

    if (sqlite3_prepare_v2(database, sql, -1, &statement, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 2, age.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 3, gender.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 4, phone.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 5, symptoms.c_str(), -1, SQLITE_TRANSIENT);

    bool success = sqlite3_step(statement) == SQLITE_DONE;
    sqlite3_finalize(statement);
    return success;
}

bool saveHealth(const std::string &name, const std::string &temperature,
                const std::string &pulse, const std::string &bp,
                const std::string &oxygen, const std::string &notes)
{
    const char *sql = "INSERT INTO health_records(patient_name, temperature, pulse, blood_pressure, oxygen_level, notes) VALUES(?,?,?,?,?,?)";
    sqlite3_stmt *statement = nullptr;

    if (sqlite3_prepare_v2(database, sql, -1, &statement, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 2, temperature.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 3, pulse.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 4, bp.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 5, oxygen.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 6, notes.c_str(), -1, SQLITE_TRANSIENT);

    bool success = sqlite3_step(statement) == SQLITE_DONE;
    sqlite3_finalize(statement);
    return success;
}

std::string readPatients()
{
    std::ostringstream html;
    html << "<h2>Patient History</h2><table><tr><th>ID</th><th>Name</th><th>Age</th><th>Gender</th><th>Phone</th><th>Symptoms</th></tr>";

    sqlite3_stmt *statement = nullptr;
    const char *sql = "SELECT id, name, age, gender, phone, symptoms FROM patients ORDER BY id DESC";

    if (sqlite3_prepare_v2(database, sql, -1, &statement, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(statement) == SQLITE_ROW)
        {
            html << "<tr>";
            for (int column = 0; column < 6; ++column)
            {
                const unsigned char *value = sqlite3_column_text(statement, column);
                std::string cell = value ? reinterpret_cast<const char *>(value) : "";
                html << "<td>" << escapeHtml(cell) << "</td>";
            }
            html << "</tr>";
        }
    }
    sqlite3_finalize(statement);
    html << "</table>";
    return html.str();
}

std::string readHealthRecords()
{
    std::ostringstream html;
    html << "<h2>Previous Health Records</h2><table><tr><th>ID</th><th>Patient</th><th>Temperature</th><th>Pulse</th><th>BP</th><th>Oxygen</th><th>Notes</th><th>Date</th></tr>";

    sqlite3_stmt *statement = nullptr;
    const char *sql = "SELECT id, patient_name, temperature, pulse, blood_pressure, oxygen_level, notes, recorded_at FROM health_records ORDER BY id DESC";

    if (sqlite3_prepare_v2(database, sql, -1, &statement, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(statement) == SQLITE_ROW)
        {
            html << "<tr>";
            for (int column = 0; column < 8; ++column)
            {
                const unsigned char *value = sqlite3_column_text(statement, column);
                std::string cell = value ? reinterpret_cast<const char *>(value) : "";
                html << "<td>" << escapeHtml(cell) << "</td>";
            }
            html << "</tr>";
        }
    }
    sqlite3_finalize(statement);
    html << "</table>";
    return html.str();
}

int main()
{
    if (sqlite3_open("cdss.db", &database) != SQLITE_OK)
    {
        std::cerr << "Could not open SQLite database.\n";
        return 1;
    }

    runSql("CREATE TABLE IF NOT EXISTS patients (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, age TEXT, gender TEXT, phone TEXT, symptoms TEXT)");
    runSql("CREATE TABLE IF NOT EXISTS health_records (id INTEGER PRIMARY KEY AUTOINCREMENT, patient_name TEXT, temperature TEXT, pulse TEXT, blood_pressure TEXT, oxygen_level TEXT, notes TEXT, recorded_at DATETIME DEFAULT CURRENT_TIMESTAMP)");

    app().registerHandler("/", [](const HttpRequestPtr &, Callback &&callback) {
        std::string content = R"(
<h2>Welcome to CDSS</h2>
<p>A simple educational system for storing patient details and recording health parameters.</p>
<div class="card"><h3>Patient Registration</h3><p>Add a new demo patient record.</p><a class="button" href="/register">Open</a></div>
<div class="card"><h3>Patient History</h3><p>View previously saved patient records.</p><a class="button" href="/history">Open</a></div>
<div class="card"><h3>Health Monitoring</h3><p>Record basic health parameters.</p><a class="button" href="/monitoring">Open</a></div>
<div class="card"><h3>Clinical Analysis</h3><p>Try a simple symptom keyword demonstration.</p><a class="button" href="/analysis">Open</a></div>
<div class="warning"><b>Disclaimer:</b> This project is for education only. It does not provide medical diagnosis or treatment.</div>)";
        sendPage(callback, "CDSS Home", content);
    }, {Get});

    app().registerHandler("/register", [](const HttpRequestPtr &request, Callback &&callback) {
        if (request->method() == Post)
        {
            std::string name = request->getParameter("name");
            std::string age = request->getParameter("age");
            std::string gender = request->getParameter("gender");
            std::string phone = request->getParameter("phone");
            std::string symptoms = request->getParameter("symptoms");

            bool saved = !name.empty() && savePatient(name, age, gender, phone, symptoms);
            std::string message = saved ? "<div class='notice'>Patient record saved successfully.</div>" :
                                           "<div class='warning'>Could not save record. Please check the form.</div>";
            sendPage(callback, "Patient Registration", message + "<a class='button' href='/register'>Add Another</a> <a class='button' href='/history'>View History</a>");
            return;
        }

        std::string content = R"(
<h2>Patient Registration</h2>
<form method="POST" action="/register">
<label>Patient Name</label><input name="name" required>
<label>Age</label><input type="number" name="age" min="0" max="120" required>
<label>Gender</label><select name="gender" required><option value="">Select</option><option>Male</option><option>Female</option><option>Other</option></select>
<label>Phone</label><input name="phone" type="tel">
<label>Symptoms</label><textarea name="symptoms"></textarea>
<br><br><button type="submit">Save Patient</button>
</form>)";
        sendPage(callback, "Patient Registration", content);
    }, {Get, Post});

    app().registerHandler("/history", [](const HttpRequestPtr &, Callback &&callback) {
        sendPage(callback, "Patient History", readPatients());
    }, {Get});

    app().registerHandler("/monitoring", [](const HttpRequestPtr &request, Callback &&callback) {
        if (request->method() == Post)
        {
            bool saved = saveHealth(request->getParameter("patient_name"),
                                    request->getParameter("temperature"),
                                    request->getParameter("pulse"),
                                    request->getParameter("blood_pressure"),
                                    request->getParameter("oxygen_level"),
                                    request->getParameter("notes"));
            std::string message = saved ? "<div class='notice'>Health record saved successfully.</div>" :
                                           "<div class='warning'>Could not save health record.</div>";
            sendPage(callback, "Health Monitoring", message +
                     R"(<a class="button" href="/monitoring">Back to Health Monitoring</a>)" +
                     readHealthRecords());
            return;
        }

        std::string content = R"(
<h2>Health Monitoring</h2>
<form method="POST" action="/monitoring">
<label>Patient Name</label><input name="patient_name" required>
<label>Temperature (°C)</label><input type="number" step="0.1" name="temperature" required>
<label>Pulse Rate (BPM)</label><input type="number" name="pulse" required>
<label>Blood Pressure</label><input name="blood_pressure" placeholder="Example: 120/80" required>
<label>Oxygen Level (%)</label><input type="number" name="oxygen_level" min="0" max="100" required>
<label>Notes</label><textarea name="notes"></textarea>
<br><br><button type="submit">Save Health Record</button>
</form>)" + readHealthRecords();
        sendPage(callback, "Health Monitoring", content);
    }, {Get, Post});

    app().registerHandler("/analysis", [](const HttpRequestPtr &request, Callback &&callback) {
        std::string result;
        if (request->method() == Post)
        {
            std::string symptoms = request->getParameter("symptoms");
            for (char &ch : symptoms)
                ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));

            if (symptoms.find("fever") != std::string::npos)
                result = "The word fever was entered. This is not a diagnosis; seek qualified clinical assessment.";
            else if (symptoms.find("cough") != std::string::npos)
                result = "The word cough was entered. This is not a diagnosis; seek qualified clinical assessment.";
            else if (symptoms.find("headache") != std::string::npos)
                result = "The word headache was entered. This is not a diagnosis; seek qualified clinical assessment.";
            else
                result = "Symptoms were recorded for demonstration. Please consult a qualified healthcare professional.";
        }

        std::string content = R"(
<h2>Clinical Analysis – Educational Demo</h2>
<form method="POST" action="/analysis">
<label>Enter Symptoms</label><textarea name="symptoms" required></textarea>
<br><br><button type="submit">Check Keywords</button>
</form>)";

        if (!result.empty())
            content += "<div class='notice'>" + escapeHtml(result) + "</div>";

        content += "<div class='warning'>This simple keyword demonstration is not a medical diagnosis and must not be used to make treatment decisions.</div>";
        sendPage(callback, "Clinical Analysis", content);
    }, {Get, Post});

    std::cout << "CDSS server running at http://127.0.0.1:8080\n";
    app().addListener("127.0.0.1", 8080).run();

    sqlite3_close(database);
    return 0;
}
