// ResInsight-specific enhancements for generated Doxygen indexes.
(function () {
    "use strict";

    function pageName() {
        return window.location.pathname.split("/").pop() || "index.html";
    }

    function rowKey(row) {
        return row.id.replace(/^row_/, "").replace(/_$/, "");
    }

    function ancestors(key) {
        const parts = key.split("_");
        const result = [];
        while (parts.length > 1) {
            parts.pop();
            result.push(parts.join("_"));
        }
        return result;
    }

    function resolveMainPageLinks() {
        if (pageName() !== "index.html") return;

        const generatedLinks = new Map();
        document.querySelectorAll(".contents a.el[title][href]").forEach(link => {
            if (!generatedLinks.has(link.title)) generatedLinks.set(link.title, link.href);
        });

        document.querySelectorAll("a[data-ri-ref]").forEach(link => {
            const resolved = generatedLinks.get(link.dataset.riRef);
            if (resolved) link.href = resolved;
        });
    }

    function groupTopLevelClasses(table) {
        const originalRows = Array.from(table.querySelectorAll("tr[id^='row_']"));
        const containerKeys = new Set();
        originalRows.forEach((row, index) => {
            const key = rowKey(row);
            const next = originalRows[index + 1];
            if (next && rowKey(next).startsWith(key + "_")) containerKeys.add(key);
        });

        const buckets = new Map();
        originalRows.forEach(row => {
            const key = rowKey(row);
            const icon = row.querySelector(".icon")?.textContent.trim();
            if (key.includes("_") || containerKeys.has(key) || icon === "N") return;

            const name = row.querySelector("td.entry a.el")?.textContent.trim() || "";
            const initial = (name.charAt(0).toLocaleUpperCase().match(/[A-Z]/) || ["#"])[0];
            if (!buckets.has(initial)) buckets.set(initial, []);
            buckets.get(initial).push(row);
        });

        const host = table.tBodies[0] || table;
        Array.from(buckets.keys()).sort().forEach(initial => {
            const members = buckets.get(initial);
            const slug = initial === "#" ? "other" : initial.toLocaleLowerCase();
            const key = "ri_alpha_" + slug;
            const group = document.createElement("tr");
            group.id = "row_" + key + "_";
            group.className = "ri-tree-letter-group";
            group.innerHTML =
                '<td class="entry"><span style="width:16px;display:inline-block;">&nbsp;</span>' +
                '<span class="ri-tree-letter-label">' + initial + "</span></td>" +
                '<td class="desc">' + members.length + " top-level classes</td>";
            host.append(group);

            members.forEach((row, index) => {
                row.id = "row_" + key + "_" + index + "_";
                host.append(row);
            });
        });
    }

    function enhanceTreeIndex() {
        const page = pageName();
        if (page !== "annotated.html" && page !== "files.html") return;

        const table = document.querySelector("div.directory > table.directory");
        if (!table) return;

        if (page === "annotated.html") groupTopLevelClasses(table);

        document.body.classList.add("ri-tree-index");
        document.body.classList.add(page === "files.html" ? "ri-file-index" : "ri-class-index");

        const rows = Array.from(table.querySelectorAll("tr[id^='row_']"));
        const rowByKey = new Map(rows.map(row => [rowKey(row), row]));
        const containerKeys = new Set();

        rows.forEach((row, index) => {
            const key = rowKey(row);
            const next = rows[index + 1];
            if (next && rowKey(next).startsWith(key + "_")) containerKeys.add(key);
        });

        const collapsed = new Set(containerKeys);
        const toolbar = document.createElement("div");
        toolbar.className = "ri-tree-toolbar";
        toolbar.innerHTML =
            '<label class="ri-tree-filter"><span>Filter</span><input type="search" placeholder="Type a namespace, class, folder, or file…" aria-label="Filter this index"></label>' +
            '<div class="ri-tree-actions"><button type="button" data-action="collapse">Collapse all</button><button type="button" data-action="expand">Expand all</button></div>' +
            '<span class="ri-tree-status" aria-live="polite"></span>';
        table.closest("div.directory").before(toolbar);

        const input = toolbar.querySelector("input");
        const status = toolbar.querySelector(".ri-tree-status");

        function update() {
            const query = input.value.trim().toLocaleLowerCase();
            const visibleForSearch = new Set();

            if (query) {
                rows.forEach(row => {
                    if (row.textContent.toLocaleLowerCase().includes(query)) {
                        const key = rowKey(row);
                        visibleForSearch.add(key);
                        ancestors(key).forEach(parent => visibleForSearch.add(parent));
                    }
                });
            }

            let visibleCount = 0;
            rows.forEach(row => {
                const key = rowKey(row);
                const hiddenByParent = ancestors(key).some(parent => collapsed.has(parent));
                const visible = query ? visibleForSearch.has(key) : !hiddenByParent;
                row.hidden = !visible;
                if (visible) {
                    row.classList.remove("odd", "even");
                    row.classList.add(visibleCount % 2 ? "odd" : "even");
                    visibleCount += 1;
                }

                const button = row.querySelector(":scope > td.entry > .ri-tree-toggle");
                if (button) {
                    const expanded = !collapsed.has(key);
                    button.setAttribute("aria-expanded", String(expanded));
                    button.title = expanded ? "Collapse this group" : "Expand this group";
                }
            });
            status.textContent = visibleCount + " of " + rows.length + " items";
        }

        containerKeys.forEach(key => {
            const row = rowByKey.get(key);
            const entry = row && row.querySelector("td.entry");
            if (!entry) return;

            const button = document.createElement("button");
            button.type = "button";
            button.className = "ri-tree-toggle";
            button.setAttribute("aria-label", "Toggle child items");
            button.addEventListener("click", event => {
                event.preventDefault();
                event.stopPropagation();
                collapsed.has(key) ? collapsed.delete(key) : collapsed.add(key);
                update();
            });
            const indent = entry.firstElementChild;
            indent ? indent.after(button) : entry.prepend(button);
        });

        toolbar.addEventListener("click", event => {
            const action = event.target.closest("button[data-action]")?.dataset.action;
            if (action === "collapse") {
                containerKeys.forEach(key => collapsed.add(key));
                input.value = "";
                update();
            } else if (action === "expand") {
                collapsed.clear();
                input.value = "";
                update();
            }
        });
        input.addEventListener("input", update);
        update();
    }

    function enhanceOverviewIndexes() {
        const page = pageName();
        const descriptions = {
            "pages.html": "Long-form guides explain architecture, ownership, extension points, automation, and efficient code navigation.",
            "topics.html": "Browse ResInsight by architectural responsibility, then drill into framework and rendering modules.",
            "concepts.html": "Language constraints and recurring design ideas that connect the project model, data layer, commands, and rendering pipeline."
        };
        if (!descriptions[page]) return;

        document.body.classList.add("ri-card-index", "ri-" + page.replace(".html", "-index"));
        const text = document.querySelector(".contents > .textblock");
        if (text) text.innerHTML = '<div class="ri-index-intro"><span>Developer index</span><p>' + descriptions[page] + "</p></div>";

        if (page !== "concepts.html") return;
        const directory = document.querySelector(".contents > div.directory");
        if (!directory) return;

        const section = document.createElement("section");
        section.className = "ri-concept-section";
        section.innerHTML =
            '<h2>Architectural concepts</h2><div class="ri-concept-grid">' +
            '<a class="ri-concept-card" href="pages.html"><strong>Layered architecture</strong><span>Understand responsibility and dependency direction.</span></a>' +
            '<a class="ri-concept-card" href="pages.html"><strong>Ownership and update flow</strong><span>Follow data from readers through models to views.</span></a>' +
            '<a class="ri-concept-card" href="pages.html"><strong>Extension patterns</strong><span>Add model objects, commands, readers, and visuals.</span></a>' +
            '<a class="ri-concept-card" href="topics.html"><strong>Automation boundaries</strong><span>Use gRPC, Python, sockets, and command files.</span></a>' +
            '<a class="ri-concept-card" href="topics.html"><strong>PDM reflection</strong><span>Persistence, fields, editors, and object ownership.</span></a>' +
            '<a class="ri-concept-card" href="topics.html"><strong>Scene and rendering</strong><span>CVF scene graphs and the visualization pipeline.</span></a>' +
            "</div></section>";
        directory.before(section);
    }

    document.addEventListener("DOMContentLoaded", function () {
        resolveMainPageLinks();
        enhanceTreeIndex();
        enhanceOverviewIndexes();
    });
})();
