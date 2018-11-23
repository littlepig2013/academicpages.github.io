---
layout: archive
title: "Publications"
permalink: /publications/
author_profile: true
---

{% if author.googlescholar %}
  You can also find my articles on <u><a href="{{author.googlescholar}}">my Google Scholar profile</a>.</u>
{% endif %}

{% include base_path %}

{% for post in site.publications reversed %}
  {% include archive-single.html %}
{% endfor %}


<b>[Evaluate Top-k Meta Path Queries on Large Heterogeneous Information Networks](http://littlepig2013.github.com/files/top-k.pdf)</b> <br>
 <b>Zichen Zhu</b>, Reynold Cheng, Loc Do, Zhipeng Huang, Haoci Zhang. <i> ICDM 2018.</i>

<b>[Design Continuums and the Path Toward Self-Designing Key-Value Stores that Know and Learn. ]() </b> <br>
Stratos Idreos, Niv Dayan, Wilson Qin, Mali Akmanalp, Sophie Hilgard, Andrew Ross, James Lennon, Varun Jain, Harshita Gupta, David Li, <b> Zichen Zhu <b> .  <i> CIDR 2019.</i>
