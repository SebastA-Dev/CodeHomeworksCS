# Proyecto Final
<hr>
Este proyecto se basa en la creacion de uns sistema el cual genere aleatoriamente tarjetones para representaciones de alcaldes y presidentes

## Clases

<div style="font-family: monospace; border:2px solid #333; border-radius:10px; padding:16px; margin-bottom:20px; background:#f8f9fa;">
  <h3 style="text-align:center; margin-top:0;">REGION</h3>
  <hr>
  <ul style="list-style:none; padding-left:10px;">
    <li><b>- nombre:</b> String</li>
    <li><b>- ciudades:</b> List&lt;Ciudad&gt;</li>
    <li><b>- censoElectoral:</b> int</li>
  </ul>
  <hr>
  <ul style="list-style:none; padding-left:10px;">
    <li><b>+ calcularCensoElectoral():</b> int</li>
  </ul>
</div>

---

<div style="font-family: monospace; border:2px solid #333; border-radius:10px; padding:16px; margin-bottom:20px; background:#f8f9fa;">
  <h3 style="text-align:center; margin-top:0;">CIUDAD</h3>
  <hr>
  <ul style="list-style:none; padding-left:10px;">
    <li><b>- nombre:</b> String</li>
    <li><b>- region:</b> Region</li>
    <li><b>- censoElectoral:</b> int</li>
  </ul>
</div>

---

<div style="font-family: monospace; border:2px solid #333; border-radius:10px; padding:16px; margin-bottom:20px; background:#f8f9fa;">
  <h3 style="text-align:center; margin-top:0;">PARTIDO</h3>
  <hr>
  <ul style="list-style:none; padding-left:10px;">
    <li><b>- nombre:</b> String</li>
    <li><b>- representanteLegal:</b> String</li>
  </ul>
</div>

---

<div style="font-family: monospace; border:2px solid #333; border-radius:10px; padding:16px; margin-bottom:20px; background:#f8f9fa;">
  <h3 style="text-align:center; margin-top:0;">CANDIDATO</h3>
  <hr>
  <ul style="list-style:none; padding-left:10px;">
    <li><b>- nombre:</b> String</li>
    <li><b>- apellido:</b> String</li>
    <li><b>- numeroIdentificacion:</b> String</li>
    <li><b>- sexo:</b> int</li>
    <li><b>- estadoCivil:</b> String</li>
    <li><b>- fechaNacimiento:</b> Date</li>
    <li><b>- ciudadNacimiento:</b> String</li>
    <li><b>- ciudadResidencia:</b> String</li>
    <li><b>- partido:</b> Partido</li>
    <li><b>- tipoCandidatura:</b> int</li>
    <li><b>- formulaVicepresidencial:</b> Candidato (opcional)</li>
  </ul>
</div>

---

## Estructura de Datos

- Lista de ciudades
- Lista de partidos legalmente reconocidos
- Lista de candidatos por ciudad
- Lista de candidatos a presidencia y vicepresidencia
- Lista de candidatos alcaldia por partido
- Lista de candidatos a presidencia y vicepresidencia por partido
