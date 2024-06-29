const baseUrl = 'http://127.0.0.1:2777/api/';

const availableModsDiv = document.getElementById('available');
const installedModsDiv = document.getElementById('installed');
const version = document.getElementById('version');


const availableModsHTML = `
<div class="available-mod" id="{{MOD_CARD_ID}}">
                <sl-card class="available-mod-card">
                    <img slot="image" src="{{MOD_IMAGE_URL}}" alt="Placeholder image" style="width: 100%; height: 200px; object-fit: cover;" />

                    <strong>{{MOD_TITLE}}</strong><br />
                    <small>{{MOD_AUTHOR}}</small><br />
                    <small>{{MOD_DESC}}</small><br />

                    <div slot="footer">
                        <sl-select style="width: 180px; height: 40px;" id="{{SELECT_ID}}">
                            {{SELECT_OPTIONS}}
                        </sl-select>
                        <sl-button style="width: 80px; height: 40px;" onClick="installMod('{{DOWNLOAD_MODID}}')">Install</sl-button>
                    </div>
            </div>
`

const installedModsHTML = `
<div class="installed-mod" id={{MOD_CARD_ID}}>
                <sl-card class="installed-mod-card">
                    <strong>{{MOD_TITLE}}</strong><br />
                    <small>{{MOD_VERSION}}</small><br />

                    <div slot="footer">
                        <sl-button style="width: 80px; height: 40px; margin-right: 10px;" onClick="uninstallMod('{{MOD_ID}}')">Remove</sl-button>
                        <sl-button style="width: 80px; height: 40px;" onClick="updateMod('{{MOD_ID_2}}')" {{UPDATE_DISABLED}}>Update</sl-button>
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
    const url = `${baseUrl}getMods`;
    const image404 = `/assets/404.jpg`

    const response = await fetch(url);
    const data = await response.json();
    const mods = data.mods;

    availableModsDiv.innerHTML = '<h2>Available Mods</h2>';

    mods.forEach(mod => {
        let selectOptions = '';
        mod.versions.forEach(version => {
            selectOptions += `<sl-menu-item value="${version.version}">${version.version}</sl-menu-item>`;
        });


        const html = availableModsHTML
            .replace('{{MOD_CARD_ID}}', "available" + mod.id)
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
    const mods = data.mods;

    installedModsDiv.innerHTML = '<h2>Installed Mods</h2>';

    mods.forEach(mod => {
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

fetchInstalledMods();