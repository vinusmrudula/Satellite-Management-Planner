from flask import Flask, jsonify
from flask_cors import CORS
import subprocess
import re
import os

app = Flask(__name__)
CORS(app)


# ============================================================
# RUN C++ PROGRAM
# ============================================================

def get_cpp_output():

    cpp_folder = os.path.abspath(
        os.path.join(
            os.path.dirname(__file__),
            "..",
            "cpp"
        )
    )

    exe_path = os.path.join(
        cpp_folder,
        "satellite.exe"
    )

    result = subprocess.run(
        [exe_path],
        input="n\n",
        capture_output=True,
        text=True,
        cwd=cpp_folder
    )

    return result.stdout


# ============================================================
# TEST C++ CONNECTION
# ============================================================

@app.route("/test")
def test_cpp():

    output = get_cpp_output()

    return "<pre>" + output + "</pre>"


# ============================================================
# SATELLITES API
# ============================================================

@app.route("/api/satellites")
def get_satellites():

    output = get_cpp_output()

    satellites = []


    # --------------------------------------------------------
    # Get only satellite section
    # --------------------------------------------------------

    if "========== PLANNED MISSIONS ==========" in output:

        satellite_section = output.split(
            "========== PLANNED MISSIONS =========="
        )[0]

    else:

        satellite_section = output


    # --------------------------------------------------------
    # Split satellite records
    # --------------------------------------------------------

    blocks = re.split(
        r"\n--- Satellite ---",
        satellite_section
    )


    for block in blocks:

        block = block.strip()


        if not block:
            continue


        satellite = {}


        # ----------------------------------------------------
        # Read each line
        # ----------------------------------------------------

        for line in block.splitlines():

            line = line.strip()


            if line.startswith("ID:"):

                satellite["id"] = (
                    line.replace("ID:", "", 1).strip()
                )


            elif line.startswith("Name:"):

                satellite["name"] = (
                    line.replace("Name:", "", 1).strip()
                )


            elif line.startswith("Type:"):

                satellite["type"] = (
                    line.replace("Type:", "", 1).strip()
                )


            elif line.startswith("Battery:"):

                value = (
                    line
                    .replace("Battery:", "", 1)
                    .replace("%", "")
                    .strip()
                )

                try:
                    satellite["battery"] = float(value)

                except ValueError:
                    satellite["battery"] = 0


            elif line.startswith("Orbit:"):

                satellite["orbit"] = (
                    line.replace("Orbit:", "", 1).strip()
                )


            elif line.startswith("Launch Date:"):

                satellite["launchDate"] = (
                    line
                    .replace("Launch Date:", "", 1)
                    .strip()
                )


            elif line.startswith("Operator:"):

                satellite["operator"] = (
                    line
                    .replace("Operator:", "", 1)
                    .strip()
                )


            elif line.startswith("Description:"):

                satellite["description"] = (
                    line
                    .replace("Description:", "", 1)
                    .strip()
                )


            elif line.startswith("Mission Purpose:"):

                satellite["missionPurpose"] = (
                    line
                    .replace("Mission Purpose:", "", 1)
                    .strip()
                )


            elif line.startswith("Sensor:"):

                satellite["sensor"] = (
                    line
                    .replace("Sensor:", "", 1)
                    .strip()
                )


            elif line.startswith("Resolution:"):

                value = (
                    line
                    .replace("Resolution:", "", 1)
                    .replace("m", "")
                    .strip()
                )

                try:
                    satellite["resolution"] = float(value)

                except ValueError:
                    satellite["resolution"] = 0


            elif line.startswith("Temperature:"):

                value = (
                    line
                    .replace("Temperature:", "", 1)
                    .replace("C", "")
                    .strip()
                )

                try:
                    satellite["temperature"] = float(value)

                except ValueError:
                    satellite["temperature"] = 0


            elif line.startswith("Humidity:"):

                value = (
                    line
                    .replace("Humidity:", "", 1)
                    .replace("%", "")
                    .strip()
                )

                try:
                    satellite["humidity"] = float(value)

                except ValueError:
                    satellite["humidity"] = 0


            elif line.startswith("Bandwidth:"):

                value = (
                    line
                    .replace("Bandwidth:", "", 1)
                    .replace("Mbps", "")
                    .strip()
                )

                try:
                    satellite["bandwidth"] = float(value)

                except ValueError:
                    satellite["bandwidth"] = 0


            elif line.startswith("AI Model:"):

                satellite["aiModel"] = (
                    line
                    .replace("AI Model:", "", 1)
                    .strip()
                )


        # ----------------------------------------------------
        # Add only valid satellite records
        # ----------------------------------------------------

        if "id" in satellite:

            satellites.append(satellite)


    return jsonify(satellites)


# ============================================================
# MISSIONS API
# ============================================================

@app.route("/api/missions")
def get_missions():

    output = get_cpp_output()

    missions = []


    # --------------------------------------------------------
    # Find mission records
    # --------------------------------------------------------

    pattern = re.compile(
        r"Mission ID:\s*(.*?)\n"
        r"Mission Name:\s*(.*?)\n"
        r"Target Location:\s*(.*?)\n"
        r"Objective:\s*(.*?)\n"
        r"Status:\s*(.*?)\n"
        r"Launch Date:\s*(.*?)\n"
        r"Estimated Duration:\s*(.*?)\n"
        r"Description:\s*(.*?)\n"
        r"Sensor Type:\s*(.*?)\n",
        re.DOTALL
    )


    matches = pattern.findall(output)


    for match in matches:

        mission_id = match[0].strip()
        mission_name = match[1].strip()
        target = match[2].strip()
        objective = match[3].strip()
        status = match[4].strip()
        launch_date = match[5].strip()
        duration = match[6].strip()
        description = match[7].strip()
        sensor = match[8].strip()


        missions.append({

            "id": mission_id,

            "name": mission_name,

            "target": target,

            "objective": objective,

            "status": status,

            "launchDate": launch_date,

            "estimatedDuration": duration,

            "description": description,

            "sensor": sensor

        })


    return jsonify(missions)


# ============================================================
# RUN FLASK
# ============================================================

if __name__ == "__main__":

    app.run(
        debug=True
    )



if __name__ == "__main__":
    app.run(debug=True)