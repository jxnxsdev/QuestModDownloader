const orginUrl = window.location.origin;
const baseUrl = `${orginUrl}/api/`;

const availableModsDiv = document.getElementById('available');
const installedModsDiv = document.getElementById('installed');
const version = document.getElementById('version');
const alert = document.getElementById('alert');
const spinner = document.getElementById('spinner');

let inProgress = false;

let mods = [];
let installedMods = [];

const availableModsHTML = `
<div class="available-mod" id="{{MOD_CARD_ID}}">
                <sl-card class="available-mod-card">
                    <img slot="image" src="{{MOD_IMAGE_URL}}" alt="Placeholder image" style="width: 100%; height: 200px; object-fit: cover;" />

                    <strong>{{MOD_TITLE}}</strong><br />
                    <small>{{MOD_AUTHOR}}</small><br />
                    <small>{{MOD_DESC}}</small><br />

                    <div slot="footer">
                        <select id="{{SELECT_ID}}" style="width: 180px; height: 40px; margin-right: 10px;">
                            {{SELECT_OPTIONS}}
                        </select>
                        <sl-button style="width: 80px; height: 40px;" variant="primary" onClick="installMod('{{DOWNLOAD_MODID}}')">Install</sl-button>
                    </div>
            </div>
`

const installedModsHTML = `
<div class="installed-mod" id={{MOD_CARD_ID}}>
                <sl-card class="installed-mod-card">
                    <strong>{{MOD_TITLE}}</strong><br />
                    <small>{{MOD_VERSION}}</small><br />

                    <div slot="footer">
                        <sl-button style="width: 80px; height: 40px; margin-right: 10px;" variant="danger" onClick="uninstallMod('{{MOD_ID}}')">Remove</sl-button>
                        <sl-button style="width: 80px; height: 40px;" variant="warning" onClick="updateMod('{{MOD_ID_2}}')" {{UPDATE_DISABLED}}>Update</sl-button>
                    </div>
                </sl-card>
            </div>
`

async function apply() {
    const url = `${baseUrl}restartGame`;
    fetch(url);
}

async function fetchVersion() {
    const url = `${baseUrl}getGameVersion`;
    const response = await fetch(url);
    const data = await response.json();
    version.innerText = data.version;
}

fetchVersion();

async function fetchAvailableMods() {
    await fetchInstalledMods();

    const url = `${baseUrl}getMods`;
    const image404 = `/assets/404.jpg`

    const response = await fetch(url);
    const data = await response.json();
    mods = data.mods;

    availableModsDiv.innerHTML = '<h2>Available Mods</h2>';

    mods.forEach(mod => {
        if (installedMods.find(installedMod => installedMod.name === mod.id)) {
            return;
        }

        if (installedMods.find(installedMod => installedMod.name === mod.name)) {
            return;
        }


        let selectOptions = '';
        mod.versions.reverse();
        mod.versions.forEach(version => {
            //selectOptions += `<sl-menu-item value="${version.version}">${version.version}</sl-menu-item>`;
            selectOptions += `<option value="${version.version}">${version.version}</option>`;
        });


        const html = availableModsHTML
            .replace('{{MOD_CARD_ID}}', "available-" + mod.id)
            .replace('{{MOD_TITLE}}', mod.name)
            .replace('{{MOD_AUTHOR}}', mod.author)
            .replace('{{MOD_DESC}}', mod.description)
            .replace('{{SELECT_ID}}', `select-${mod.id}`)
            .replace('{{SELECT_OPTIONS}}', selectOptions)
            .replace('{{DOWNLOAD_MODID}}', mod.id)
            .replace('{{MOD_IMAGE_URL}}', mod.cover || image404);

        availableModsDiv.innerHTML += html;
    });
}

fetchAvailableMods();


async function fetchInstalledMods() {
    const url = `${baseUrl}getLocalMods`;

    const response = await fetch(url);
    const data = await response.json();
    installedMods = data.mods;



    installedModsDiv.innerHTML = '<h2>Installed Mods</h2>';

    installedMods.forEach(mod => {
        if(!mod.name.includes('com.beatgames.beatsaber')) {
            const html = installedModsHTML
                .replace('{{MOD_CARD_ID}}', "installed-" + mod.name)
                .replace('{{MOD_TITLE}}', mod.name)
                .replace('{{MOD_VERSION}}', mod.version)
                .replace('{{MOD_ID}}', mod.name)
                .replace('{{MOD_ID_2}}', mod.name)
                .replace('{{UPDATE_DISABLED}}', mod.updateAvailable ? '' : 'disabled');

            installedModsDiv.innerHTML += html;
        }
    });
}

async function getBusy() {
    const url = `${baseUrl}isBusy`;
    const response = await fetch(url);
    const data = await response.json();
    const isBusy = data.isBusy;
    inProgress = isBusy;

    if(isBusy) {
        showAlert("Mod installation is currently in Progress, please wait...", "danger", 10000);
        spinner.style.display = 'flex';
    } else {
        spinner.style.display = 'none';
    }
}

getBusy();

async function showAlert(message, type = "primary", duration = 5000) {
    alert.variant = type;
    alert.innerText = message;
    alert.open = true;
    alert.duration = duration;
}

async function reloadMods() {
    console.log('reloading mods');
    await fetchAvailableMods();
}