// ============================================================
// SATELLITE MANAGEMENT SYSTEM - FRONTEND JAVASCRIPT
// ============================================================


// ============================================================
// API URLS
// ============================================================

const SATELLITE_API =
    "http://127.0.0.1:5000/api/satellites";

const MISSION_API =
    "http://127.0.0.1:5000/api/missions";


// ============================================================
// GLOBAL VARIABLES
// ============================================================

let satellites = [];
let missions = [];

let currentSatellitePage = 1;

const satellitesPerPage = 12;

let currentFilter = "ALL";

let currentMissionPage = 1;

const missionsPerPage = 6;
// ============================================================
// LOAD DATA WHEN PAGE OPENS
// ============================================================

document.addEventListener(
    "DOMContentLoaded",
    function () {

        loadSatellites();

        loadMissions();

    }
);


// ============================================================
// LOAD SATELLITES
// ============================================================

async function loadSatellites() {

    try {

        const response =
            await fetch(SATELLITE_API);


        if (!response.ok) {

            throw new Error(
                "Unable to load satellites."
            );

        }


        satellites =
            await response.json();


        console.log(
            "Loaded satellites:",
            satellites.length
        );


        displaySatellites();


    }

    catch (error) {

        console.error(
            "Satellite loading error:",
            error
        );


        const container =
            document.getElementById(
                "satellite-container"
            );


        if (container) {

            container.innerHTML = `
                <p style="color:red;">
                    Unable to load satellites.
                    Make sure Flask is running.
                </p>
            `;

        }

    }

}


// ============================================================
// DISPLAY SATELLITES
// ============================================================

function displaySatellites() {

    const container =
        document.getElementById(
            "satellite-container"
        );


    if (!container) {

        return;

    }


    let filteredSatellites =
        satellites;


    // --------------------------------------------------------
    // APPLY FILTER
    // --------------------------------------------------------

    if (currentFilter !== "ALL") {

        filteredSatellites =
            satellites.filter(
                satellite =>
                    satellite.type &&
                    satellite.type
                        .toUpperCase()
                        .includes(
                            currentFilter
                        )
            );

    }


    // --------------------------------------------------------
    // PAGINATION
    // --------------------------------------------------------

    const totalPages =
        Math.ceil(
            filteredSatellites.length /
            satellitesPerPage
        );


    if (
        currentSatellitePage >
        totalPages
    ) {

        currentSatellitePage =
            1;

    }


    const startIndex =
        (currentSatellitePage - 1) *
        satellitesPerPage;


    const endIndex =
        startIndex +
        satellitesPerPage;


    const pageSatellites =
        filteredSatellites.slice(
            startIndex,
            endIndex
        );


    // --------------------------------------------------------
    // CLEAR CONTAINER
    // --------------------------------------------------------

    container.innerHTML = "";


    // --------------------------------------------------------
    // NO RESULTS
    // --------------------------------------------------------

    if (
        pageSatellites.length === 0
    ) {

        container.innerHTML = `
            <p>
                No satellites found.
            </p>
        `;

        return;

    }


    // --------------------------------------------------------
    // CREATE SATELLITE CARDS
    // --------------------------------------------------------

    pageSatellites.forEach(
        satellite => {

            const card =
                document.createElement(
                    "div"
                );


            card.className =
                "satellite-card";


            card.innerHTML = `

                <div class="satellite-card-content">

                    <h3>
                        ${safeText(
                            satellite.name ||
                            "Unknown Satellite"
                        )}
                    </h3>


                    <p>
                        <strong>ID:</strong>
                        ${safeText(
                            satellite.id || "-"
                        )}
                    </p>


                    <p>
                        <strong>Type:</strong>
                        ${safeText(
                            satellite.type || "-"
                        )}
                    </p>


                    <p>
                        <strong>Orbit:</strong>
                        ${safeText(
                            satellite.orbit || "-"
                        )}
                    </p>


                    <p>
                        <strong>Battery:</strong>
                        ${
                            satellite.battery !==
                            undefined
                                ? satellite.battery +
                                  "%"
                                : "-"
                        }
                    </p>


                    <button
                        class="view-details-btn"
                        onclick="openSatelliteDetails('${escapeQuotes(satellite.id)}')">

                        View Details

                    </button>

                </div>

            `;


            container.appendChild(
                card
            );

        }
    );


    // --------------------------------------------------------
    // PAGINATION
    // --------------------------------------------------------

    displaySatellitePagination(
        totalPages
    );

}


// ============================================================
// SATELLITE PAGINATION
// ============================================================

function displaySatellitePagination(
    totalPages
) {

    const pagination =
        document.getElementById(
            "satellite-pagination"
        );


    if (!pagination) {

        return;

    }


    pagination.innerHTML = "";


    if (totalPages <= 1) {

        return;

    }


    // Previous button

    const previousButton =
        document.createElement(
            "button"
        );


    previousButton.textContent =
        "← Previous";


    previousButton.disabled =
        currentSatellitePage === 1;


    previousButton.onclick =
        function () {

            if (
                currentSatellitePage >
                1
            ) {

                currentSatellitePage--;

                displaySatellites();

            }

        };


    pagination.appendChild(
        previousButton
    );


    // Page numbers

    for (
        let i = 1;
        i <= totalPages;
        i++
    ) {

        const button =
            document.createElement(
                "button"
            );


        button.textContent =
            i;


        if (
            i ===
            currentSatellitePage
        ) {

            button.classList.add(
                "active"
            );

        }


        button.onclick =
            function () {

                currentSatellitePage =
                    i;

                displaySatellites();

            };


        pagination.appendChild(
            button
        );

    }


    // Next button

    const nextButton =
        document.createElement(
            "button"
        );


    nextButton.textContent =
        "Next →";


    nextButton.disabled =
        currentSatellitePage ===
        totalPages;


    nextButton.onclick =
        function () {

            if (
                currentSatellitePage <
                totalPages
            ) {

                currentSatellitePage++;

                displaySatellites();

            }

        };


    pagination.appendChild(
        nextButton
    );

}


// ============================================================
// SEARCH SATELLITES
// ============================================================

function searchSatellites() {

    const searchBox =
        document.getElementById(
            "satellite-search"
        );


    if (!searchBox) {

        return;

    }


    const searchText =
        searchBox.value
            .toLowerCase()
            .trim();


    const container =
        document.getElementById(
            "satellite-container"
        );


    if (!container) {

        return;

    }


    const filtered =
        satellites.filter(
            satellite => {

                return (

                    (satellite.name || "")
                        .toLowerCase()
                        .includes(searchText)

                    ||

                    (satellite.id || "")
                        .toLowerCase()
                        .includes(searchText)

                    ||

                    (satellite.type || "")
                        .toLowerCase()
                        .includes(searchText)

                    ||

                    (satellite.orbit || "")
                        .toLowerCase()
                        .includes(searchText)

                );

            }
        );


    displaySatelliteSearchResults(
        filtered
    );

}


// ============================================================
// DISPLAY SEARCH RESULTS
// ============================================================

function displayMissions() {

    const container = document.getElementById("mission-container");

    if (!container) {
        return;
    }

    container.innerHTML = "";

    if (!missions || missions.length === 0) {

        container.innerHTML = `
            <div style="
                grid-column: 1 / -1;
                text-align: center;
                padding: 50px;
                color: #aaa;
            ">
                No missions available.
            </div>
        `;

        return;
    }


    // -----------------------------
    // PAGINATION
    // -----------------------------

    const startIndex =
        (currentMissionPage - 1) * missionsPerPage;

    const endIndex =
        startIndex + missionsPerPage;

    const currentMissions =
        missions.slice(startIndex, endIndex);


    // -----------------------------
    // CREATE MISSION CARDS
    // -----------------------------

    currentMissions.forEach(mission => {

        const card =
            document.createElement("div");

        card.className = "mission-card";


        card.innerHTML = `

            <div class="mission-card-content">

                <div style="
                    color: #8aa8c7;
                    font-size: 13px;
                    margin-bottom: 10px;
                    letter-spacing: 1px;
                ">
                    ${safeText(mission.id || "MISSION")}
                </div>


                <h3 style="
                    margin-bottom: 15px;
                ">
                    ${safeText(
                        mission.name ||
                        "Unnamed Mission"
                    )}
                </h3>


                <p style="
                    color: #aebbd0;
                    margin-bottom: 20px;
                    line-height: 1.5;
                ">
                    Satellite mission for
                    <strong>
                        ${safeText(
                            mission.target ||
                            "specified target"
                        )}
                    </strong>.
                </p>


                <button
                    class="view-details-btn"
                    type="button"
                >
                    View Details →
                </button>

            </div>

        `;


        // -----------------------------
        // BUTTON
        // -----------------------------

        const detailsButton =
            card.querySelector(
                ".view-details-btn"
            );


        detailsButton.addEventListener(
            "click",
            function () {

                openMissionDetails(
                    mission.id
                );

            }
        );


        container.appendChild(card);

    });


    // -----------------------------
    // PAGINATION
    // -----------------------------

    const totalPages =
        Math.ceil(
            missions.length /
            missionsPerPage
        );


    displayMissionPagination(totalPages);

}

function displayMissionPagination(
    totalPages
) {

    const pagination =
        document.getElementById(
            "mission-pagination"
        );

    if (!pagination) {
        return;
    }

    pagination.innerHTML = "";


    if (totalPages <= 1) {
        return;
    }


    const previousButton =
        document.createElement(
            "button"
        );

    previousButton.textContent =
        "← Previous";

    previousButton.disabled =
        currentMissionPage === 1;


    previousButton.onclick =
        function () {

            if (
                currentMissionPage >
                1
            ) {

                currentMissionPage--;

                displayMissions();

            }

        };


    pagination.appendChild(
        previousButton
    );


    for (
        let i = 1;
        i <= totalPages;
        i++
    ) {

        const button =
            document.createElement(
                "button"
            );

        button.textContent = i;


        if (
            i ===
            currentMissionPage
        ) {

            button.classList.add(
                "active"
            );

        }


        button.onclick =
            function () {

                currentMissionPage =
                    i;

                displayMissions();

            };


        pagination.appendChild(
            button
        );

    }


    const nextButton =
        document.createElement(
            "button"
        );

    nextButton.textContent =
        "Next →";

    nextButton.disabled =
        currentMissionPage ===
        totalPages;


    nextButton.onclick =
        function () {

            if (
                currentMissionPage <
                totalPages
            ) {

                currentMissionPage++;

                displayMissions();

            }

        };


    pagination.appendChild(
        nextButton
    );
}


// ============================================================
// FILTER SATELLITES
// ============================================================

function filterSatellites(
    filter
) {

    currentFilter =
        filter.toUpperCase();


    currentSatellitePage =
        1;


    displaySatellites();

}


// ============================================================
// SATELLITE DETAILS
// ============================================================

function openSatelliteDetails(
    satelliteId
) {

    if (!satelliteId) {

        return;

    }


    window.location.href =
        "satellite-details.html?id=" +
        encodeURIComponent(
            satelliteId
        );

}


// ============================================================
// LOAD MISSIONS
// ============================================================

async function loadMissions() {

    try {

        const response =
            await fetch(MISSION_API);


        if (!response.ok) {

            throw new Error(
                "Unable to load missions."
            );

        }


        missions =
            await response.json();


        console.log(
            "Loaded missions:",
            missions.length
        );


        displayMissions();

    }

    catch (error) {

        console.error(
            "Mission loading error:",
            error
        );


        const container =
            document.getElementById(
                "mission-container"
            );


        if (container) {

            container.innerHTML = `
                <p style="color:red;">
                    Unable to load missions.
                    Make sure Flask is running.
                </p>
            `;

        }

    }

}


// ============================================================
// DISPLAY MISSIONS
// ============================================================

function displayMissions() {

    const container = document.getElementById("mission-container");

    if (!container) {
        return;
    }

    container.innerHTML = "";

    if (!missions || missions.length === 0) {

        container.innerHTML = `
            <div style="
                grid-column: 1 / -1;
                text-align: center;
                padding: 50px;
                color: #aaa;
            ">
                No missions available.
            </div>
        `;

        return;
    }


    // ==============================
    // PAGINATION
    // ==============================

    const startIndex =
        (currentMissionPage - 1) * missionsPerPage;

    const endIndex =
        startIndex + missionsPerPage;

    const currentMissions =
        missions.slice(startIndex, endIndex);


    // ==============================
    // CREATE CARDS
    // ==============================

    currentMissions.forEach(mission => {

        const card = document.createElement("div");

        card.className = "mission-card";


        card.innerHTML = `

            <div class="mission-card-content">

                <div style="
                    color: #8aa8c7;
                    font-size: 13px;
                    margin-bottom: 10px;
                    letter-spacing: 1px;
                ">
                    ${safeText(mission.id || "MISSION")}
                </div>


                <h3 style="
                    margin-bottom: 15px;
                ">
                    ${safeText(
                        mission.name || "Unnamed Mission"
                    )}
                </h3>


                <p style="
                    color: #aebbd0;
                    margin-bottom: 20px;
                    line-height: 1.5;
                ">
                    Satellite mission for
                    <strong>
                        ${safeText(
                            mission.target || "specified target"
                        )}
                    </strong>.
                </p>


                <button
                    class="view-details-btn"
                    type="button"
                >
                    View Details →
                </button>

            </div>

        `;


        const button =
            card.querySelector(".view-details-btn");


        button.addEventListener("click", function () {

            openMissionDetails(mission.id);

        });


        container.appendChild(card);

    });


    // ==============================
    // PAGINATION
    // ==============================

    const totalPages =
        Math.ceil(
            missions.length / missionsPerPage
        );


    displayMissionPagination(totalPages);

}

// ============================================================
// MISSION DETAILS
// ============================================================

function openMissionDetails(missionId) {

    console.log(
        "Opening mission:",
        missionId
    );


    if (!missionId) {

        console.error(
            "Mission ID is missing."
        );

        return;
    }


    const url =
        "mission-details.html?id=" +
        encodeURIComponent(
            String(missionId).trim()
        );


    console.log(
        "Opening URL:",
        url
    );


    window.location.assign(url);

}


// ============================================================
// ADMIN LOGIN
// ============================================================

function openAdminLogin() {

    const modal =
        document.getElementById(
            "admin-login-modal"
        );


    if (modal) {

        modal.style.display =
            "flex";

    }

}


// ============================================================
// CLOSE ADMIN LOGIN
// ============================================================

function closeAdminLogin() {

    const modal =
        document.getElementById(
            "admin-login-modal"
        );


    if (modal) {

        modal.style.display =
            "none";

    }

}


// ============================================================
// ADMIN LOGIN CHECK
// ============================================================

function loginAdmin() {

    const usernameInput =
        document.getElementById(
            "admin-username"
        );


    const passwordInput =
        document.getElementById(
            "admin-password"
        );


    if (
        !usernameInput ||
        !passwordInput
    ) {

        return;

    }


    const username =
        usernameInput.value.trim();


    const password =
        passwordInput.value;


    /*
     * TEMPORARY FRONTEND LOGIN
     *
     * We will later connect this
     * properly to the C++ Admin class.
     */

    if (
        username === "admin" &&
        password === "isro_1234"
    ) {

        sessionStorage.setItem(
            "adminLoggedIn",
            "true"
        );


        window.location.href =
            "admin.html";

    }

    else {

        alert(
            "Invalid username or password."
        );

    }

}


// ============================================================
// LOGOUT ADMIN
// ============================================================

function logoutAdmin() {

    sessionStorage.removeItem(
        "adminLoggedIn"
    );


    window.location.href =
        "index.html";

}


// ============================================================
// ADMIN PAGE PROTECTION
// ============================================================

function checkAdminAccess() {

    const loggedIn =
        sessionStorage.getItem(
            "adminLoggedIn"
        );


    if (
        loggedIn !== "true"
    ) {

        window.location.href =
            "index.html";

    }

}


// ============================================================
// UTILITY - SAFE TEXT
// ============================================================

function safeText(
    value
) {

    return String(value)
        .replaceAll("&", "&amp;")
        .replaceAll("<", "&lt;")
        .replaceAll(">", "&gt;")
        .replaceAll('"', "&quot;")
        .replaceAll("'", "&#039;");

}


// ============================================================
// UTILITY - ESCAPE QUOTES
// ============================================================

function escapeQuotes(
    value
) {

    return String(value)
        .replaceAll(
            "\\",
            "\\\\"
        )
        .replaceAll(
            "'",
            "\\'"
        );

}


// ============================================================
// CLOSE MODAL WHEN CLICKING OUTSIDE
// ============================================================

window.addEventListener(
    "click",
    function (event) {

        const modal =
            document.getElementById(
                "admin-login-modal"
            );


        if (
            modal &&
            event.target === modal
        ) {

            closeAdminLogin();

        }

    }
);