async function installMod(modId) {
    if (inProgress) {
        return;
    }

    inProgress = true;

    const version = document.getElementById(`select-${modId}`);
    const versionValue = version.options[version.selectedIndex].value;
    
    const mod = mods.find(mod => mod.id === modId);
    const modVersion = mod.versions.find(version => version.version === versionValue);

    const downloadUrl = modVersion.download;

    console.log(`Installing ${modId} version ${versionValue} from ${downloadUrl}`);
    showAlert(`Installing ${mod.name} with version ${versionValue}`);

    // lock all interactions
    spinner.style.display = 'flex';

    const url = `${baseUrl}installMod`;

    const response = await fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({
            modId,
            version: versionValue,
            downloadUrl,
        }),
    });

    const data = await response.json();

    if (data.success) {
        showAlert(`Successfully installed ${mod.name} with version ${versionValue}`, "success", 5000);
    } else {
        showAlert(`Failed to install ${mod.name} with version ${versionValue}\n\n${data.message}`, "danger", 5000);
    }

    setTimeout(() => {
        getBusy();
        reloadMods();
    }, 1000);
}

async function uninstallMod(modId) {
    if (inProgress) {
        return;
    }

    inProgress = true;

    showAlert(`Uninstalling ${modId}...`);

    spinner.style.display = 'flex';

    const url = `${baseUrl}uninstallMod`;

    const response = await fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({
            modId,
        }),
    });

    const data = await response.json();

    if (data.success) {
        showAlert(`Successfully uninstalled ${modId}`, "success", 5000);
    } else {
        showAlert(`Failed to uninstall ${modId}\n\n${data.message}`, "danger", 5000);
    }

    setTimeout(() => {
        getBusy();
        reloadMods();
    }, 1000);
}

async function updateMod(modId) {
    if (inProgress) {
        return;
    }

    inProgress = true;

    showAlert(`Updating ${modId}...`);

    spinner.style.display = 'flex';

    const url = `${baseUrl}updateMod`;

    const response = await fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({
            modId,
        }),
    });

    const data = await response.json();

    if (data.success) {
        showAlert(`Successfully updated ${modId}`, "success", 5000);
    } else {
        showAlert(`Failed to update ${modId}\n\n${data.message}`, "danger", 5000);
    }

    setTimeout(() => {
        getBusy();
        reloadMods();
    }, 1000);
}