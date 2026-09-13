#ifndef WEB_PAGE_H
#define WEB_PAGE_H

#include <Arduino.h>

// Hela dashboarden ligger i flashminnet (PROGMEM) som en enda sida.
// Medvetet val: inga externa filer, teckensnitt eller CDN-bibliotek.
// Sidan maste fungera nar enheten saknar internet - da gar inget att hamta
// utifran. Allt (HTML, CSS och den lilla mangd JavaScript som uppdaterar
// varden) levereras darfor fran ESP32:n sjalv i ett enda HTTP-svar.

const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="sv">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>MicroHydros</title>
<style>
  :root{
    --botten:#071410;
    --panel:#0f211c;
    --linje:#1d3830;
    --text:#e8f3ec;
    --dov:#86a79a;
    --vaxtljus:#ff5fb0;
    --vatten:#57c7e8;
    --varning:#f0b849;
  }
  *{box-sizing:border-box}
  body{
    margin:0;
    padding:24px 20px 48px;
    background:var(--botten);
    color:var(--text);
    font-family:-apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,Helvetica,Arial,sans-serif;
    line-height:1.5;
    -webkit-font-smoothing:antialiased;
  }
  main{max-width:860px;margin:0 auto}

  /* Statusremsan ar sidans viktigaste element: den svarar pa fragan
     "gar mina matvarden till molnet just nu, eller visas de bara har?" */
  .status{
    border:1px solid var(--linje);
    border-left:6px solid var(--vaxtljus);
    border-radius:4px;
    background:var(--panel);
    padding:16px 18px;
    display:flex;
    gap:14px;
    align-items:baseline;
    flex-wrap:wrap;
  }
  .status.direkt{border-left-color:#4fd39a}
  .status.lokalt{border-left-color:var(--varning)}
  .status.borta{border-left-color:#e2585b}
  .status strong{font-size:1.15rem;font-weight:600}
  .status span{color:var(--dov);font-size:.92rem}

  h1{font-size:1.4rem;font-weight:600;margin:32px 0 2px;letter-spacing:-.01em}
  .undertext{color:var(--dov);font-size:.9rem;margin:0 0 24px}

  .matvarden{
    display:grid;
    grid-template-columns:repeat(auto-fit,minmax(160px,1fr));
    gap:1px;
    background:var(--linje);
    border:1px solid var(--linje);
    border-radius:4px;
    overflow:hidden;
  }
  .matvarde{background:var(--panel);padding:18px}
  .matvarde h2{margin:0;font-size:.85rem;font-weight:500;color:var(--dov)}
  .tal{
    font-size:2.4rem;
    font-weight:600;
    font-variant-numeric:tabular-nums;
    letter-spacing:-.02em;
    margin-top:6px;
  }
  .tal .enhet{font-size:1rem;font-weight:400;color:var(--dov);margin-left:3px}
  .inne .tal{color:var(--vaxtljus)}
  .vatten .tal{color:var(--vatten)}
  .saknas .tal{color:#e2585b;font-size:1.6rem}

  dl.fakta{
    display:grid;
    grid-template-columns:repeat(auto-fit,minmax(150px,1fr));
    gap:14px 24px;
    margin:24px 0 0;
    padding-top:20px;
    border-top:1px solid var(--linje);
    font-size:.9rem;
  }
  dl.fakta dt{color:var(--dov)}
  dl.fakta dd{margin:2px 0 0;font-variant-numeric:tabular-nums}

  h2.rubrik{font-size:1rem;font-weight:600;margin:36px 0 4px}
  .rubriktext{color:var(--dov);font-size:.85rem;margin:0 0 12px}
  table{width:100%;border-collapse:collapse;font-size:.88rem;font-variant-numeric:tabular-nums}
  th{text-align:right;font-weight:500;color:var(--dov);padding:8px 10px;border-bottom:1px solid var(--linje)}
  th:first-child{text-align:left}
  td{text-align:right;padding:8px 10px;border-bottom:1px solid rgba(29,56,48,.55)}
  td:first-child{text-align:left;color:var(--dov)}
  tr:last-child td{border-bottom:none}
  .tom{color:var(--dov);font-size:.9rem;padding:14px 0}

  footer{margin-top:36px;color:var(--dov);font-size:.8rem}
  @media (max-width:520px){
    .tal{font-size:2rem}
    body{padding:16px 14px 40px}
  }
</style>
</head>
<body>
<main>
  <div class="status" id="status">
    <strong id="statusText">Hämtar status från enheten</strong>
    <span id="statusDetalj"></span>
  </div>

  <h1>MicroHydros</h1>
  <p class="undertext" id="undertext">Lokal vy direkt från odlingsenheten</p>

  <section class="matvarden">
    <div class="matvarde inne" id="k-tempInne">
      <h2>Lufttemperatur inne</h2>
      <div class="tal">–<span class="enhet">°C</span></div>
    </div>
    <div class="matvarde inne" id="k-fuktInne">
      <h2>Luftfuktighet inne</h2>
      <div class="tal">–<span class="enhet">%</span></div>
    </div>
    <div class="matvarde" id="k-tempUte">
      <h2>Lufttemperatur ute</h2>
      <div class="tal">–<span class="enhet">°C</span></div>
    </div>
    <div class="matvarde vatten" id="k-tempVatten">
      <h2>Näringslösning</h2>
      <div class="tal">–<span class="enhet">°C</span></div>
    </div>
  </section>

  <dl class="fakta">
    <div><dt>Nätverk</dt><dd id="f-natverk">–</dd></div>
    <div><dt>Adress</dt><dd id="f-ip">–</dd></div>
    <div><dt>Skickat till molnet</dt><dd id="f-skickat">–</dd></div>
    <div><dt>Missade sändningar</dt><dd id="f-missade">–</dd></div>
    <div><dt>Igång sedan start</dt><dd id="f-uptime">–</dd></div>
    <div><dt>Mätning nummer</dt><dd id="f-seq">–</dd></div>
  </dl>

  <h2 class="rubrik">Senaste mätningarna</h2>
  <p class="rubriktext" id="rubriktext">Finns bara i enheten och försvinner vid omstart.</p>
  <table>
    <thead>
      <tr><th>Tid</th><th>Inne °C</th><th>Inne %RH</th><th>Ute °C</th><th>Vatten °C</th></tr>
    </thead>
    <tbody id="historik"></tbody>
  </table>
  <p class="tom" id="tomText">Väntar på första mätningen.</p>

  <footer>Uppdateras var femte sekund. Data hämtas från /api/status på enheten.</footer>
</main>

<script>
const nollLangd = v => String(v).padStart(2,'0');

function tid(rad){
  if(rad.ts && rad.ts > 0){
    const d = new Date(rad.ts*1000);
    return nollLangd(d.getHours())+':'+nollLangd(d.getMinutes())+':'+nollLangd(d.getSeconds());
  }
  return '+'+uptime(rad.uptime_s);
}

function uptime(s){
  const t = Math.floor(s/3600), m = Math.floor((s%3600)/60);
  return t > 0 ? t+' h '+m+' min' : m+' min';
}

function visaVarde(id, varde, decimaler, enhet){
  const kort = document.getElementById(id);
  const tal = kort.querySelector('.tal');
  if(varde === null || varde === undefined){
    kort.classList.add('saknas');
    tal.innerHTML = 'Ingen avläsning';
  }else{
    kort.classList.remove('saknas');
    tal.innerHTML = varde.toFixed(decimaler)+'<span class="enhet">'+enhet+'</span>';
  }
}

function visaStatus(d){
  const ruta = document.getElementById('status');
  const text = document.getElementById('statusText');
  const detalj = document.getElementById('statusDetalj');
  ruta.className = 'status';

  if(d.mqtt.ansluten){
    ruta.classList.add('direkt');
    text.textContent = 'Mätvärden skickas till molnet';
    detalj.textContent = d.mqtt.skickade+' mätningar skickade sedan start';
  }else{
    ruta.classList.add('lokalt');
    text.textContent = 'Ingen kontakt med molnet';
    detalj.textContent = 'Mätningarna visas bara här så länge avbrottet varar. '
      + d.mqtt.ej_skickade+' mätningar har hoppats över.';
  }
}

function visaHistorik(rader){
  const kropp = document.getElementById('historik');
  const tom = document.getElementById('tomText');
  kropp.innerHTML = '';
  if(!rader.length){ tom.style.display='block'; return; }
  tom.style.display='none';
  rader.slice().reverse().forEach(r=>{
    const tr = document.createElement('tr');
    const c = v => v === null || v === undefined ? '–' : v.toFixed(1);
    tr.innerHTML = '<td>'+tid(r)+'</td><td>'+c(r.temp_inne_c)+'</td><td>'+c(r.fukt_inne_rh)
      +'</td><td>'+c(r.temp_ute_c)+'</td><td>'+c(r.temp_vatten_c)+'</td>';
    kropp.appendChild(tr);
  });
}

async function uppdatera(){
  try{
    const svar = await fetch('/api/status', {cache:'no-store'});
    const d = await svar.json();

    visaStatus(d);
    const s = d.senaste;
    if(s){
      visaVarde('k-tempInne',  s.temp_inne_c,   1, '°C');
      visaVarde('k-fuktInne',  s.fukt_inne_rh,  1, '%');
      visaVarde('k-tempUte',   s.temp_ute_c,    1, '°C');
      visaVarde('k-tempVatten',s.temp_vatten_c, 1, '°C');
      document.getElementById('undertext').textContent =
        d.device_id+', senast avläst '+tid(s);
      document.getElementById('f-seq').textContent = s.seq;
    }
    document.getElementById('f-natverk').textContent = d.wifi.ap_lage
      ? 'Lokal accesspunkt' : (d.wifi.ansluten ? 'Anslutet ('+d.wifi.rssi+' dBm)' : 'Söker nätverk');
    document.getElementById('f-ip').textContent = d.wifi.ip;
    document.getElementById('f-skickat').textContent = d.mqtt.skickade;
    document.getElementById('f-missade').textContent = d.mqtt.ej_skickade;
    document.getElementById('f-uptime').textContent = uptime(d.uptime_s);
    document.getElementById('rubriktext').textContent =
      'De '+d.lokal_historik.kapacitet+' senaste mätningarna, sparade i enheten. Försvinner vid omstart.';
    visaHistorik(d.historik || []);
  }catch(e){
    const ruta = document.getElementById('status');
    ruta.className = 'status borta';
    document.getElementById('statusText').textContent = 'Ingen kontakt med enheten';
    document.getElementById('statusDetalj').textContent =
      'Kontrollera att du är ansluten till samma nätverk som MicroHydros.';
  }
}

uppdatera();
setInterval(uppdatera, 5000);
</script>
</body>
</html>
)rawliteral";

#endif