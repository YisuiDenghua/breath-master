#include <KColorScheme>
#include <KConfig>
#include <KConfigGroup>
#include <KIconThemes/KIconTheme>
#include <KSharedDataCache>
#include <KSycoca>


#include <QDebug>
#include <QHash>
#include <QDir>
#include <QProcess>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>

static QHash<QStringList, QString> s_colors = {
    {{
      "Breath2Light", "Breath22021Light","Breath22021BLight","Breath22021CLight",
      "Breath2", "Breath22021","Breath22021B","Breath22021C"       
     }, "BreathLight"},
    {{
       "Breath22021HighContrast", "Breath22021BHighContrast", "Breath22021CHighContrast", //highcontrast
         "Breath2Dark", "Breath22021Dark","Breath22021BDark","Breath22021CDark"
    }, "BreathDark"},
};

static QHash<QStringList, QString> s_lnfs = {
    {{"org.manjaro.breath2.desktop","org.manjaro.breath2-2021.desktop"},"org.manjaro.breath.desktop"},
    {{"org.manjaro.breath2-2021-light.desktop"},"org.manjaro.breath-light.desktop"},
    {{"org.manjaro.breath2-2021-dark.desktop"},"org.manjaro.breath-dark.desktop"}
};

static QHash<QStringList, QString> s_plasmaThemes = {
    {{"breath2","breath-color"},"breath"},
    {{"breath2-light", "breath2-2021-light"},"breath-light"},
    {{"breath2-dark", "breath2-2021-dark"},"breath-dark"},
};

static QHash<QStringList, QString> s_iconThemes = {
    {{"breath2"},"breeze"},
    {{"breath2-dark"},"breeze-dark"}
};

static QHash<QStringList, QString> s_splash = {
    {{"org.manjaro.breath2-2021.desktop","org.manjaro.breath2.desktop",
        "org.kde.breath.desktop", "org.kde.breath2.desktop"}, "org.manjaro.breath.desktop"}
};

static QHash<QStringList, QString> s_yakuakeThemes = {
    {{"breath2"}, "breath-light"},
    {{"breath2_dark"}, "breath-dark"}
};

QString findTargetThemeName(const QString& configName,
                  const QString& groupName,
                  const QString& entryName,
                  const QHash<QStringList, QString>& migration){
    
    KConfig config(configName);
    KConfigGroup group(&config, groupName);
    auto entry = group.readEntry(entryName);
    auto it = migration.constKeyValueBegin();
    while(it!=migration.constKeyValueEnd()
           && !it->first.contains(entry)){
        it++;
        
    }
    if(it==migration.constKeyValueEnd()){
        return "";
    }
    return it->second;
}

QString migrate(const QString& configName,
             const QString& groupName,
             const QString& entryName,
             const QHash<QStringList, QString>& migration){
    KConfig config(configName);
    KConfigGroup group(&config, groupName);
    auto entry = group.readEntry(entryName);
    
    auto it = migration.constKeyValueBegin();
    while(it!=migration.constKeyValueEnd()
           && !it->first.contains(entry)){
        it++;
    }
    if(it==migration.constKeyValueEnd()){
        return "";
    }        
    QString targetName = it->second;
    
    group.writeEntry(entryName, targetName, KConfig::Notify);
    
    return targetName;
}

void updateColors(){
    QString target = findTargetThemeName("kdeglobals", "General", "ColorScheme", s_colors);
    
    if(target.isEmpty()){
        return;
    }
    
    QString targetColorPath = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("color-schemes/")+target+".colors");
    if (targetColorPath.isEmpty()) {
        return;
    }
    QProcess::startDetached("/bin/plasma-apply-colorscheme", {target});
}
void updateLnFs(){
    migrate("kdeglobals", "KDE", "LookAndFeelPackage", s_lnfs);
}
void updatePlasmaThemes(){
    migrate("plasmarc", "Theme", "name", s_plasmaThemes);
}

void updateIcons(){
    migrate("kdeglobals", "Icons", "Theme", s_iconThemes);
    
    KIconTheme::reconfigure();
    
    KSharedDataCache::deleteCache(QStringLiteral("icon-cache"));
    
    for (int i = 0; i < KIconLoader::LastGroup; i++) {
        KIconLoader::emitChange(KIconLoader::Group(i));
    }
}

void updateSplash(){
    migrate("ksplashrc", "KSplash", "Theme", s_splash);
}

void updateYakuake(){
    migrate("yakuakerc", "Appearance", "Skin", s_yakuakeThemes);
}

void updateWallpaper(){
    QVariant script =
                    "var Desktops = desktops();\n"
                    "for (i=0;i<Desktops.length;i++) {\n"
                    "    d = Desktops[i];\n"
                    "    d.wallpaperPlugin = \"org.kde.image\";\n"
                    "    d.currentConfigGroup = Array(\"Wallpaper\",\n"
                    "                                \"org.kde.image\",\n"
                    "                                \"General\");\n"
                    "    let wp = d.readConfig(\"Image\");\n"
                    "    if(wp.includes(\"/usr/share/wallpapers/Breath2\")){\n"
                    "        d.writeConfig(\"Image\", \"file:///usr/share/wallpapers/Bamboo\")\n"
                    "    }\n"
                    "}\n";

    auto message = QDBusMessage::createMethodCall("org.kde.plasmashell", "/PlasmaShell", "org.kde.PlasmaShell", "evaluateScript");
    message.setArguments({script});
    QDBusConnection::sessionBus().call(message);
}

int main()
{
    updatePlasmaThemes();
    updateIcons();
    updateColors();    
    updateLnFs();
    updateSplash();
    updateWallpaper();
    updateYakuake();
    
    KSycoca().ensureCacheValid();
}
