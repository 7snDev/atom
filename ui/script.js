const title = document.getElementById("title");
const input = document.getElementById("input");

document.addEventListener("DOMContentLoaded", async () => {
  title.textContent = ( await window.cppbinded() || {"title":"Hello, World!"} ).title;
})

title.addEventListener("click", () => {
  const value = input.value;
  input.value = title.textContent;
  title.textContent = value;
  window.cpplog(value);
});